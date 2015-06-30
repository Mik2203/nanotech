#include "rosystemsolver.h"

#include <QDebug>

#define _USE_MATH_DEFINES // for MSVC 2010 C++
#include <math.h>
#include <limits.h>
#include "romath.h"

#include <QElapsedTimer>


#ifdef QT_DEBUG
#include <iostream>
#include <fstream>

#endif


qint64 SET_MATRIX_TIME;
qint64 SOLVE_MATRIX_TIME;
int CALC_MATRIX_COUNT;

template<typename Derived>
inline bool has_nan(const Eigen::MatrixBase<Derived>& x) { return !x.isApprox(x); }

template<typename Derived>
inline bool has_inf(const Eigen::MatrixBase<Derived>& x) { return !(x-x).isApprox(x-x); }

inline bool is_nan(double v) {
    volatile double temp = v;
    return temp != v;
}
inline bool is_inf(double v) {
    volatile double temp = v;
    return ((temp == v) && ((temp - v) != 0.0));
}

ROSystemSolver::ROSystemSolver(ROSystem * const sys, QObject *parent) :
    QObject(parent),
    _elementEquationCount(0),
    _passEquationCount(0),
    _tolerance(0.01),
    _setSystemValues(true),
//    _decomposition(ColPivHouseholderQR),
    _sys(sys)
{
}


bool ROSystemSolver::solved() const { return _solved; }

double ROSystemSolver::tolerance() const { return _tolerance; }

void ROSystemSolver::setSystemValues() {
    double sysPf = 0.0;
    for (int pi=0; pi<_sys->passCount(); ++pi) {
        ROPass* pass = _sys->pass(pi);
        ROStage* firstStage = pass->firstStage();

        double pPp = 0.0;

        firstStage->rawWater()->solutes()->beginChange();
        firstStage->rawWater()->solutes()->reset();
        for (int sii=0; sii<_usedSolutes.count(); ++sii) {
            int si = _usedSolutes[sii];
            firstStage->rawWater()->solutes()->setMeql(si, s1sCf(pi, sii));
        }
        firstStage->rawWater()->solutes()->setPH(s1PHf(pi));
        firstStage->rawWater()->setTemperature(T);
        firstStage->rawWater()->solutes()->endChange();

        firstStage->rawWater()->setPressure(ePf(pi, 0));

//        firstStage->rawWater()->


        Eigen::VectorXd sCPQP = Eigen::VectorXd::Zero(_usedSolutes.count()); // els count
        double sQP = 0.0;

        for(int si = 0, ei = 0; si < pass->stageCount(); ++si) {

            ROStage* stage = pass->stage(si);
            sCPQP = Eigen::VectorXd::Zero(_usedSolutes.count());
            sQP = 0.0;

            stage->rawWater()->setRate(evQf(pi, ei));

            ROElement* firstElement = stage->firstElement();
            firstElement->feed()->setRate(eQf(pi, ei));
            firstElement->feed()->setTemperature(T);
            firstElement->feed()->copySolutesFrom(stage->rawWater());
            firstElement->feed()->setPressure(ePf(pi, ei) + ePp(pi, ei));


            for (int ei2 = 0; ei2 < stage->elementsPerVesselCount(); ++ei2, ++ei) {
                sQP += eQp(pi, ei);

                stage->element(ei2)->permeate()->solutes()->beginChange();
                stage->element(ei2)->concentrate()->solutes()->beginChange();
                stage->element(ei2)->permeate()->solutes()->reset();//copySolutesFrom(stage->feed());
                stage->element(ei2)->concentrate()->solutes()->reset();//copySolutesFrom(stage->feed());


                for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                    int si = _usedSolutes[sii];
                    stage->element(ei2)->permeate()->solutes()->setMeql(si, esCp(pi, ei, sii));
                    stage->element(ei2)->concentrate()->solutes()->setMeql(si, esCc(pi, ei, sii));

                    sCPQP(sii) += esCp(pi, ei, sii) * eQp(pi, ei);
                }
                stage->element(ei2)->permeate()->setPH(ePHp(pi, ei));
                stage->element(ei2)->concentrate()->setPH(ePHc(pi, ei));

                stage->element(ei2)->permeate()->setTemperature(T);
                stage->element(ei2)->concentrate()->setTemperature(T);

                stage->element(ei2)->permeate()->solutes()->endChange();
                stage->element(ei2)->concentrate()->solutes()->endChange();

                stage->element(ei2)->concentrate()->setRate(eQc(pi, ei)/* / stage->vesselCount()*/);
                stage->element(ei2)->permeate()->setRate(eQp(pi, ei)/* / stage->vesselCount()*/);

//                stage->element(ei2)->feed()->setPressure(ePf(pi, ei) + ePp(pi, ei));
                stage->element(ei2)->permeate()->setPressure(ePb(pi));
                stage->element(ei2)->concentrate()->setPressure(ePc(pi, ei) + ePp(pi, ei));  // см ниже почему надо вычитать Pb

            }

            stage->feed()->setPressure(stage->firstElement()->feed()->pressure());
            stage->permeate()->setPressure(stage->firstElement()->permeate()->pressure());

            // В концентрате нужно вычесть обратное давление фильтрата, т.к. оно идет в фильтрат.
            // Нельзя сразу аккумулировать Pp (см расчет Pp) без Pb, именно из-за того, что Pb должно быть
            // также аккумулировано. Проще просто вычесть его из концентрата.
            stage->concentrate()->setPressure(stage->lastElement()->concentrate()->pressure());

            // Solutes
            stage->permeate()->solutes()->beginChange();
            stage->permeate()->solutes()->reset();


            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                stage->permeate()->solutes()->setMeql(si, sCPQP(sii) / sQP);
            }
            int hco3i = _usedSolutes.indexOf(ROSolutes::HCO3);
            int co2i = _usedSolutes.indexOf(ROSolutes::CO2);
            double sPHp = (hco3i >= 0) ? ph(stage->permeate()->solutes()->meql(ROSolutes::HCO3),
                                           sCPQP(co2i) / sQP, // esCp(pi, ei, co2i),
                                           T,
                                           stage->permeate()->solutes()->ionicStrength()) : sysPHaf;
            stage->permeate()->solutes()->setPH(sPHp);
            stage->permeate()->setTemperature(T);

            stage->permeate()->solutes()->endChange();

            stage->concentrate()->copySolutesFrom(stage->lastElement()->concentrate());
            stage->concentrate()->setTemperature(T);

            // Rates


            stage->concentrate()->setRate(stage->lastElement()->concentrate()->rate() * stage->vesselCount());
            stage->permeate()->setRate(sQP * stage->vesselCount());

            // Pressures
            pPp += stage->permeate()->pressure();
        }


        ROStage* lastStage = pass->lastStage();
        lastStage->concentrate()->setPressure(lastStage->lastElement()->concentrate()->pressure()/* -
                                              lastStage->backPressure() -
                                              lastStage->preStagePressure()*/);
//        lastStage->lastElement()->concentrate()->setPressure(lastStage->concentrate()->pressure());


        pass->concentrate()->copySolutesFrom(lastStage->concentrate());
        pass->concentrate()->setTemperature(T);
        double pQtr = pQsr(pi);
        Q_FOREACH(const double& r, pass->outgoingRecycles()) pQtr += r;
        pass->concentrate()->setRate(lastStage->concentrate()->rate() - pQtr);
//        pass->totalProduct()->setRate(pQpb(pi));

        // pass solutes
        pass->permeate()->solutes()->beginChange();
//        pass->totalProduct()->solutes()->beginChange();

        pass->permeate()->solutes()->reset();//copySolutesFrom(pass->feed());
//        pass->totalProduct()->solutes()->reset();//copySolutesFrom(pass->feed());

        pass->permeate()->setPH(pPHp(pi));
        pass->permeate()->setTemperature(T);
//        pass->totalProduct()->setPH(pPHpb(pi));

        for (int sii=0; sii<_usedSolutes.count(); ++sii) {
            int si = _usedSolutes[sii];
            pass->permeate()->solutes()->setMeql(si, psCp(pi, sii));
//            pass->totalProduct()->solutes()->setMeql(si, psCpb(pi, sii));

//            sysCcQc(sii) += pass->concentrate()->solutes()->meql(si) * pass->concentrate()->rate();
        }
        pass->permeate()->solutes()->endChange();
        pass->permeate()->setPressure(pPp / pass->stageCount());
//        pass->totalProduct()->solutes()->endChange();

        // pass pressures
        pass->feed()->setPressure(firstStage->feed()->pressure());
//        pass->permeate()->setPressure(lastStage->permeate()->pressure());
        pass->concentrate()->setPressure(lastStage->concentrate()->pressure());


        sysPf += pass->feed()->pressure();
    }

    _sys->feed()->setPressure(sysPf);
//    _sys->feed()->setRate();

//    qDebug() << _sys->permeate()->solutes()->totalValueMgl();
//    qDebug() << _sys->firstPass()->totalProduct()->solutes()->totalValueMgl();
}



void ROSystemSolver::solve() {
    CALC_MATRIX_COUNT = 0;
    SET_MATRIX_TIME = 0.0;
    SOLVE_MATRIX_TIME = 0.0;

//#ifdef QT_DEBUG
    QElapsedTimer t;
    t.start();
//#endif

//    _decomposition = ColPivHouseholderQR;
    _solved = init();

    if (_solved && _setSystemValues) setSystemValues();
    Q_EMIT finished();
}

void ROSystemSolver::addLogV() {
    logV.resize(logV.size()+1);
    for(int ei=0; ei<peCount(0); ++ei) {
        logV.last() << evQf(0, ei) << eQp(0, ei) << evQc(0, ei) <<
                       eCf(0, ei) << eCp(0, ei) << eCc(0, ei);
    }
}

void ROSystemSolver::printLogV() {
    int varsCount = logV.first().count();

    for(int vi=0; vi<varsCount; ++vi) {
        QString vStr;
        for (int ci=0; ci<logV.count(); ++ci) {
            vStr.append(QString::number(logV[ci][vi], 'f', 2) + "\t");
        }
        qDebug() << vStr.replace(".", ",").replace(" ", "\t");
    }
}

void ROSystemSolver::checkForNan(const Eigen::MatrixXd& mat) {
    QString nans;
    QString infs;
    if (has_nan(mat)) {
        for (int row=0; row<mat.rows(); ++row) {
            for(int col=0; col<mat.cols(); ++col) {
                if (is_nan(mat(row, col)))
                    nans.append(QString("(%1:%2, %3:%4)").arg(row).arg(logT.at(row)).arg(col).arg(logT.at(col)));
                if (is_inf(mat(row, col)))
                    infs.append(QString("(%1:%2, %3:%4)").arg(row).arg(logT.at(row)).arg(col).arg(logT.at(col)));
            }
        }
    }
    if (nans.length() > 0) qDebug() << "nans:" << nans;
    if (infs.length() > 0) qDebug() << "infs:" << infs;
}


void ROSystemSolver::zeroMatrices() {
    J.setZero();
    X.setZero(); // = Eigen::VectorXd::Zero(matrixSize); // Вектор переменных
    dX.setZero(); // = Eigen::VectorXd::Zero(matrixSize); // Вектор приращений на каждом шаге
    F.setZero(); // = Eigen::VectorXd::Zero(matrixSize); // Вектор значений функций
}

bool ROSystemSolver::init() {
    logV.clear();


    // INIT INTERNALS
    _usedSolutes = _sys->adjustedFeed()->solutes()->filledIons();
    _elementEquationCount = 3 * (_usedSolutes.count()) + 11; // (Cpi + Cci + SPmi) * (ions) + Qc + Cp + Cc + pHc + pHp + Ic + Ip + PIc + PIp + Pc + Qp
    _passEquationCount = 4 * (1 + _usedSolutes.count()) + 10; // (pCfr + s1Cf + pCp + pCpb) * (totalC + ionC) + (pPHfr + s1PHf + pPHp + pPHpb) + same Ic + s1PIf + s1Pf
    // END INIT INTERNALS

    T = _sys->temperature();

    // Подсчет количества элементов в ступени
    pQb = Eigen::VectorXd::Zero(_sys->passCount()); // blending rates
    pQfb = Eigen::VectorXd::Zero(_sys->passCount()); // flow rates after blend
    pQraw = Eigen::VectorXd::Zero(_sys->passCount()); // flow rates on pass
    pQf = Eigen::VectorXd::Zero(_sys->passCount()); // flow rates after big recycles

    sysCaf = _sys->adjustedFeed()->solutes()->totalValue(ROSolutes::Meql); // sys adjusted feed concentration
    sysIaf = _sys->adjustedFeed()->solutes()->ionicStrength();
    sysPHaf = _sys->adjustedFeed()->solutes()->pH();
    syssCaf = Eigen::VectorXd::Zero(_usedSolutes.count());
    _preComputedICoeffs = Eigen::VectorXd::Zero(_usedSolutes.count());
    for (int sii=0; sii<_usedSolutes.count(); ++sii) {
        int si = _usedSolutes[sii];
        syssCaf(sii) = _sys->adjustedFeed()->solutes()->meql(si);
        _preComputedICoeffs(sii) = abs(ROSolutes::ionicCharge(si)) * lToKg / 2;
    }
    sysQaf = _sys->adjustedFeed()->rate(); // sys adjusted feed concentration

    pQp = Eigen::VectorXd::Zero(_sys->passCount()); // passes permeate flow rates from stages
    pQpb = Eigen::VectorXd::Zero(_sys->passCount()); // passes permeate flow rates after blending
    pQsr = Eigen::VectorXd::Zero(_sys->passCount()); // passes self recycle rate
    pQr = Eigen::VectorXd::Zero(_sys->passCount()); // passes other recycles rate
    e1vQf = Eigen::VectorXd::Zero(_sys->passCount()); // flow rates on first stage
    //s1Qp = Eigen::VectorXd::Zero(_sys->passCount()); // flow rates on first stage
    pFF = Eigen::VectorXd::Zero(_sys->passCount()); // passes flow factors
    pSPI = Eigen::VectorXd::Zero(_sys->passCount()); // passes salt passage increase

    for (int pi=0; pi < _sys->passCount(); ++pi) {
        const ROPass* const pass = _sys->pass(pi);

        pQsr[pi] = pass->hasSelfRecycle() ? pass->selfRecycle() : 0.0;
        pQr(pi) = 0.0;
        Q_FOREACH(const double& r, pass->incomingRecycles()) pQr[pi] += r;
        pQp(pi) = pass->permeate()->rate();
        pQraw[pi] = pass->rawWater()->rate();
        pFF[pi] = pass->flowFactor();
        pSPI[pi] = pow(1.0 + pass->saltPassageYearIncrease(), _sys->elementLifetime());
        pQb[pi] = pass->hasBlendPermeate() ? pass->blendPermeate() : 0.0;
        pQf[pi] = pass->feed()->rate();
        pQfb[pi] = pass->feed()->rate() - pQr(pi);
        e1vQf[pi] = (pQf[pi] + pQsr[pi]); // / pass->firstStage()->vesselCount(); // TODO nano ne nado delit' - ne pomnu
        pQpb(pi) = pQp(pi) + pQb(pi);
    }


    piCount = Eigen::VectorXi::Zero(_sys->passCount()); // index count
    peCount = Eigen::VectorXi::Zero(_sys->passCount()); // els count
    int totalElsCount = 0;
    int maxElsInPass = 0;
    piOff = Eigen::VectorXi::Zero(_sys->passCount()); // passes first index offsets
    peOff = Eigen::VectorXi::Zero(_sys->passCount()); //


    for (int pi = 0; pi < _sys->passCount(); ++pi) {


        if (pi < _sys->passCount()) {
            const ROPass* const pass = _sys->pass(pi);

            for (int si=0; si < pass->stageCount(); ++si)
                peCount[pi] += pass->stage(si)->elementsPerVesselCount();

            piCount[pi] = peCount[pi] * _elementEquationCount + _passEquationCount;
            totalElsCount += peCount[pi];

            if (pi > 0) {
                peOff[pi] = peOff[pi-1] + peCount[pi-1];
                piOff[pi] = piOff[pi-1] + piCount[pi-1];
            }

            maxElsInPass = qMax(maxElsInPass, peCount[pi]);

            for(int ei=0; ei<peCount(pi); ++ei) {
                // 1
                for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                    int si = _usedSolutes[sii];
                    logT << QString("esCp(%1, %2, %3").arg(pi).arg(ei).arg(si);
                }
                // 2
                logT << QString("eCp(%1, %2)").arg(pi).arg(ei);
                // 3
                for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                    int si = _usedSolutes[sii];
                    logT << QString("esCc(%1, %2, %3").arg(pi).arg(ei).arg(si);
                }
                // 4
                logT << QString("eCc(%1, %2)").arg(pi).arg(ei);
                // 5
                logT << QString("eQc(%1, %2)").arg(pi).arg(ei);
                // 6
                for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                    int si = _usedSolutes[sii];
                    logT << QString("esSPm(%1, %2, %3").arg(pi).arg(ei).arg(si);
                }
                // 7
                logT << QString("ePHp(%1, %2)").arg(pi).arg(ei);
                // 8
                logT << QString("ePHc(%1, %2)").arg(pi).arg(ei);
                // 9
                logT << QString("eIp(%1, %2)").arg(pi).arg(ei);
                // 10
                logT << QString("eIc(%1, %2)").arg(pi).arg(ei);
                // 11
                logT << QString("ePIp(%1, %2)").arg(pi).arg(ei);
                // 12
                logT << QString("ePIc(%1, %2)").arg(pi).arg(ei);
                // 13
                logT << QString("ePc(%1, %2)").arg(pi).arg(ei);
                // 14
                logT << QString("eQp(%1, %2)").arg(pi).arg(ei);
            }
            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                logT << QString("psCfr(%1, %2)").arg(pi).arg(si);
            }
            logT << QString("pCfr(%1)").arg(pi);
            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                logT << QString("s1sCf(%1, %2)").arg(pi).arg(si);
            }
            logT << QString("s1Cf(%1)").arg(pi);
            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                logT << QString("psCp(%1, %2)").arg(pi).arg(si);
            }
            logT << QString("pCp(%1)").arg(pi);
            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                logT << QString("psCpb(%1, %2)").arg(pi).arg(si);
            }
            logT << QString("pCp(%1)").arg(pi);
            logT << QString("pPHfr(%1)").arg(pi);
            logT << QString("s1PHf(%1)").arg(pi);
            logT << QString("pPHp(%1)").arg(pi);
            logT << QString("pPHpb(%1)").arg(pi);
            logT << QString("pIfr(%1)").arg(pi);
            logT << QString("s1If(%1)").arg(pi);
            logT << QString("s1PIf(%1)").arg(pi);
            logT << QString("s1Pf(%1)").arg(pi);
            logT << QString("pIp(%1)").arg(pi);
            logT << QString("pIpb(%1)").arg(pi);



        }
        //logT << ""
    }

    // Инициализация переменных SPm, S, Pb
    msi = Eigen::VectorXi::Zero(totalElsCount);
    s = Eigen::VectorXd::Zero(totalElsCount);
    v = Eigen::VectorXd::Zero(totalElsCount);

    pp = Eigen::VectorXd::Zero(totalElsCount);
    ePb = Eigen::VectorXd::Zero(_sys->passCount());

    for (int pi = _sys->passCount()-1, ei=totalElsCount; pi >= 0; --pi) {
        const ROPass* const pass = _sys->pass(pi);

        double preStageAccumulator = 0.0;
        for(int si=pass->stageCount()-1; si >= 0; --si) {
            const ROStage* const stage = pass->stage(si);
            ei -= stage->elementsPerVesselCount();
            preStageAccumulator += stage->preStagePressure();
            msi.segment(ei, stage->elementsPerVesselCount()) = Eigen::VectorXi::Constant(stage->elementsPerVesselCount(), stage->membrane()->seriesIndex());
            s.segment(ei, stage->elementsPerVesselCount()) = Eigen::VectorXd::Constant(stage->elementsPerVesselCount(), stage->membrane()->activeArea());
            v.segment(ei, stage->elementsPerVesselCount()) = Eigen::VectorXd::Constant(stage->elementsPerVesselCount(), stage->vesselCount());
            pp.segment(ei, stage->elementsPerVesselCount()) = Eigen::VectorXd::Constant(stage->elementsPerVesselCount(), preStageAccumulator + pass->backPressure());
        }
        ePb[pi] = pass->backPressure();
    }

    // Инициализация
    int matrixSize = totalElsCount * _elementEquationCount + _sys->passCount() * _passEquationCount;
    J = Eigen::SparseMatrix<double>(matrixSize,matrixSize); // Якобиан
    X = Eigen::VectorXd::Zero(matrixSize); // Вектор переменных
    dX = Eigen::VectorXd::Zero(matrixSize); // Вектор приращений на каждом шаге
    F = Eigen::VectorXd::Zero(matrixSize); // Вектор значений функций

    initSystem();
    return calcSystem();
}

void ROSystemSolver::initPass(int pi) {

    static const double Cp_decrease = 0.01;
    static const double Cc_increase = 1.1;
    static const double Qp_decrease = 0.1;



//    const ROPass* const pass = _sys->pass(pi);
    e1Qp(pi) = pQp(pi) / eV(pi, 0) * 0.25; // pass->elementsCount();
    e1vQc(pi) = e1vQf(pi) - e1vQp(pi);
    pCf(pi) = pCraw(pi); // real = ((pQfr-pQb)*pCf + sum(QRi*CRi)) / pQf
    s1Cf(pi) = (pQf(pi) * pCf(pi) + pQsr(pi) * pCraw(pi)) / e1vQf(pi);
    // Инициализация вектора переменных
    s1Cp(pi) = s1Cf(pi) * Cp_decrease;
    s1Cc(pi) = s1Cf(pi) * Cc_increase;
    s1PIf(pi) = PI_old(s1Cf(pi), T);
    s1PIc(pi) = PI_old(s1Cc(pi), T);
    s1PIp(pi) = PI_old(s1Cp(pi), T);

    s1Pf(pi) = Pf(e1Qf(pi), e1Qp(pi), e1Qc(pi), s1Cf(pi), s1Cc(pi), s1PIf(pi), s1PIp(pi), s1PIc(pi), e1S(pi),
                  T, pFF(pi), MSi(pi, 0));

    int hco3i = _usedSolutes.indexOf(ROSolutes::HCO3);
    int co2i = _usedSolutes.indexOf(ROSolutes::CO2);
    for (int si=0; si<_usedSolutes.count(); ++si) {
        psCf(pi, si) = psCraw(pi, si);
        s1sCf(pi, si) = (pQf(pi) * psCf(pi, si) + pQsr(pi) * psCraw(pi, si)) / e1vQf(pi);
        psCp(pi, si) = s1sCf(pi, si) * Cp_decrease;
        psCpb(pi, si)  = (pQp(pi)*psCp(pi, si) + pQb(pi)*psCraw(pi, si)) / pQpb(pi);
        if (si != co2i) {
            pIpb(pi) += psCpb(pi, si) * _preComputedICoeffs(si);
            pIp(pi) += psCp(pi, si) * _preComputedICoeffs(si);
            s1If(pi) += s1sCf(pi, si) * _preComputedICoeffs(si);
            pIf(pi) += psCf(pi, si) * _preComputedICoeffs(si);
        }
    }

    if (hco3i >= 0) {
        pPHf(pi) = ph(psCf(pi, hco3i), psCf(pi, co2i), T, pIf(pi));
        s1PHf(pi) = ph(s1sCf(pi, hco3i), s1sCf(pi, co2i), T, s1If(pi));
        pPHp(pi) = ph(psCp(pi, hco3i), psCp(pi, co2i), T, pIp(pi));
        pPHpb(pi) = ph(psCpb(pi, hco3i), psCpb(pi, co2i), T, pIpb(pi));
        pPHraw(pi) = ph(psCraw(pi, hco3i), psCraw(pi, co2i), T, pIraw(pi));
    } else {
        pPHf(pi) = s1PHf(pi) = pPHp(pi) = pPHpb(pi) = pPHraw(pi);
    }

    pCp(pi) = 0.0;

    for(int ei = 0; ei < peCount[pi]; ++ei) {

        ePHp(pi, ei) = ePHc(pi, ei) = pPHraw(pi);
        eIp(pi, ei) = eIc(pi, ei) = pIraw(pi);

        if (ei > 0) {
            eQp(pi, ei) = eQf(pi, ei) * Qp_decrease; // TODO * recovery
        }
        evQc(pi, ei) = evQf(pi, ei) - evQp(pi, ei);
        for (int sii=0; sii<_usedSolutes.count(); ++sii) {
            int si = _usedSolutes[sii];
            double Cp_k = (si == ROSolutes::CO2) ? 1.0 : Cp_decrease;
            esCp(pi, ei, sii) = esCf(pi, 0, sii) * Cp_k;

            double Cc_k = (si == ROSolutes::CO2) ? 1.0 : Cc_increase;
            esCc(pi, ei, sii) = esCf(pi, 0, sii) * Cc_k;

            if (si != ROSolutes::CO3) {
                if (si == ROSolutes::B)
                    esSPm(pi, ei, sii) = pSPI[pi] * SPmPh(MSi(pi, ei), ePHf(pi, ei));
                else
                    esSPm(pi, ei, sii) = pSPI[pi] * SPm(MSi(pi, ei), si, esCf(pi, 0, sii));
            }

        }
        eCp(pi, ei) = s1Cf(pi) * Cp_decrease;
        eCc(pi, ei) = s1Cf(pi) * Cc_increase;
        ePIc(pi, ei) = PI_old(eCc(pi, ei), T);
        ePIp(pi, ei) = PI_old(eCp(pi, ei), T);
        pCp(pi) += evQp(pi, ei) * eCp(pi, ei);

        ePc(pi, ei) = ePf(pi, ei) - dPfc(eQf(pi, ei), eQc(pi, ei));

    }

    evQc(pi, LAST_ei(pi)) = e1vQf(pi) - pQp(pi);

    pCp(pi) /= pQp(pi);
    pCpb(pi)  = (pQp(pi)*pCp(pi) + pQb(pi)*pCraw(pi)) / pQpb(pi); // real = pCp: (pQts-pQb)*pCts + pQb*pCf = pQts*pCp
}


bool ROSystemSolver::calcSystem() {
    QElapsedTimer timer;
    bool solved = false;
    int stepCntr = 0;
#ifdef QT_DEBUG
    logValues();
    std::ofstream myfile;
    myfile.open ("debug.txt");
#endif
    //    qDebug() << peCount(0) << piCount(0);
    int hco3i = _usedSolutes.indexOf(ROSolutes::HCO3);
    int co2i = _usedSolutes.indexOf(ROSolutes::CO2);

    qint64 elapsed = 0;

    while (!solved && stepCntr < 100) {

        QElapsedTimer t;
        t.start();

        timer.restart();
        //#pragma omp parallel for
        for(int pi=0; pi<_sys->passCount(); ++pi) {
            const ROPass* const pass = _sys->pass(pi);


            F[ieQp(pi, 0)] = -pQp(pi);

            //#pragma omp parallel for
            for(int ei = 0; ei < peCount(pi); ++ei) {

                // s1Qp

                J.coeffRef(ieQp(pi, 0), ieQp(pi, ei)) = eV(pi, ei);
                F[ieQp(pi, 0)] += evQp(pi, ei);

                //Ip
                J.coeffRef(ieIp(pi, ei), ieIp(pi, ei)) = 1;
                F[ieIp(pi, ei)] = eIp(pi, ei);

                //Ic
                J.coeffRef(ieIc(pi, ei), ieIc(pi, ei)) = 1;
                F[ieIc(pi, ei)] = eIc(pi, ei);

                //PHp
                J.coeffRef(iePHp(pi, ei), iePHp(pi, ei)) = 1;
                if (hco3i >= 0) {
                    J.coeffRef(iePHp(pi, ei), iesCp(pi, ei, hco3i)) = -dphhco3(esCp(pi, ei, hco3i));
                    J.coeffRef(iePHp(pi, ei), iesCp(pi, ei, co2i)) = -dphco2(esCp(pi, ei, co2i));
                    J.coeffRef(iePHp(pi, ei), ieIp(pi, ei)) = -dphi(eIp(pi, ei));
                    F[iePHp(pi, ei)] = ePHp(pi, ei) - ph(esCp(pi, ei, hco3i), esCp(pi, ei, co2i), T, eIp(pi, ei));
                } else {
                    F[iePHp(pi, ei)] = ePHp(pi, ei) - sysPHaf; //ePHf(pi, ei);
                }

                //PHc
                J.coeffRef(iePHc(pi, ei), iePHc(pi, ei)) = 1;
                if (hco3i >= 0) {
                    J.coeffRef(iePHc(pi, ei), iesCc(pi, ei, hco3i)) = -dphhco3(esCc(pi, ei, hco3i));
                    J.coeffRef(iePHc(pi, ei), iesCc(pi, ei, co2i)) = -dphco2(esCc(pi, ei, co2i));
                    J.coeffRef(iePHc(pi, ei), ieIc(pi, ei)) = -dphi(eIc(pi, ei));
                    F[iePHc(pi, ei)] = ePHc(pi, ei) - ph(esCc(pi, ei, hco3i), esCc(pi, ei, co2i), T, eIc(pi, ei));
                } else {
                    F[iePHc(pi, ei)] = ePHc(pi, ei) - sysPHaf;//ePHf(pi, ei);
                }

                //Cp
                J.coeffRef(ieCp(pi, ei), ieCp(pi, ei)) = 1;
                F[ieCp(pi, ei)] = eCp(pi, ei);

                //Cc
                J.coeffRef(ieCc(pi, ei), ieCc(pi, ei)) = 1;
                F[ieCc(pi, ei)] = eCc(pi, ei);

                //Qc
                if (ei < LAST_ei(pi)) {
                    J.coeffRef(ievQc(pi, ei), ievQc(pi, ei)) = 1;
                    J.coeffRef(ievQc(pi, ei), ieQp(pi, ei)) = eV(pi, ei);
                    if (ei > 0) {
                        J.coeffRef(ievQc(pi, ei), ievQf(pi, ei)) = -1;
                    }
                    F[ievQc(pi, ei)] = evQc(pi, ei) + evQp(pi, ei) - evQf(pi, ei);
                }

                // Qp
                if (ei > 0) {
                    J.coeffRef(ieQp(pi, ei), iePf(pi, ei)) = 1;
                    J.coeffRef(ieQp(pi, ei), ievQf(pi, ei)) = -dPfQf(eQf(pi, ei), eQp(pi, ei), eQc(pi, ei), eCf(pi, ei), eCc(pi, ei), ePIf(pi, ei), eS(pi, ei), T, pFF(pi), MSi(pi, ei)) * (-1 / (eV(pi, ei) * eV(pi, ei)));
                    J.coeffRef(ieQp(pi, ei), ieQp(pi, ei)) = -dPfQp(eQf(pi, ei), eQp(pi, ei), eCf(pi, ei), eCc(pi, ei), ePIf(pi, ei), eS(pi, ei), T, pFF(pi), MSi(pi, ei));
                    if (ei < LAST_ei(pi)) {
                        J.coeffRef(ieQp(pi, ei), ievQc(pi, ei)) = -dPfQc(eQf(pi, ei), eQc(pi, ei)) * (-1 / (eV(pi, ei) * eV(pi, ei)));
                    }
                    J.coeffRef(ieQp(pi, ei), ieCf(pi, ei)) = -dPfCf(eQf(pi, ei), eQp(pi, ei), eCf(pi, ei), eCc(pi, ei), ePIf(pi, ei), eS(pi, ei), T, pFF(pi), MSi(pi, ei));
                    J.coeffRef(ieQp(pi, ei), ieCc(pi, ei)) = -dPfCc(eQf(pi, ei), eQp(pi, ei), eCf(pi, ei), eCc(pi, ei), ePIf(pi, ei), eS(pi, ei), T, pFF(pi), MSi(pi, ei));
                    J.coeffRef(ieQp(pi, ei), iePIf(pi, ei)) = -dPfPIf();
                    J.coeffRef(ieQp(pi, ei), iePIp(pi, ei)) = -dPfPIp();
                    J.coeffRef(ieQp(pi, ei), iePIc(pi, ei)) = -dPfPIc();

                    F[ieQp(pi, ei)] = ePf(pi, ei) - Pf(eQf(pi, ei), eQp(pi, ei), eQc(pi, ei), eCf(pi, ei), eCc(pi, ei), ePIf(pi, ei), ePIp(pi, ei), ePIc(pi, ei), eS(pi, ei), T, pFF(pi), MSi(pi, ei));
                }

                // PI
                // will be used for new PI
                QMap<int, double> epSolutes;
                QMap<int, double> ecSolutes;
                for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                    int si = _usedSolutes[sii];
                    epSolutes[si] = esCp(pi, ei, sii);
                    ecSolutes[si] = esCc(pi, ei, sii);
                }
                J.coeffRef(iePIp(pi, ei), iePIp(pi, ei)) = 1;
                J.coeffRef(iePIp(pi, ei), ieCp(pi, ei)) = dPI_oldTds(T);
                F[iePIp(pi, ei)] = ePIp(pi, ei) - PI_old(eCp(pi, ei), T);

                J.coeffRef(iePIc(pi, ei), iePIc(pi, ei)) = 1;
                J.coeffRef(iePIc(pi, ei), ieCc(pi, ei)) = dPI_oldTds(T);
                F[iePIc(pi, ei)] = ePIc(pi, ei) - PI_old(eCc(pi, ei), T);

                // Pc
                J.coeffRef(iePc(pi, ei), iePc(pi, ei)) = 1;
                J.coeffRef(iePc(pi, ei), iePf(pi, ei)) = -1;
                if (ei < LAST_ei(pi)) {
                    J.coeffRef(iePc(pi, ei), ievQc(pi, ei)) = ddPfcQc(eQf(pi, ei), eQc(pi, ei)) * (-1 / (eV(pi, ei) * eV(pi, ei)));
                }
                if (ei > 0)
                    J.coeffRef(iePc(pi, ei), ievQf(pi, ei)) = ddPfcQf(eQf(pi, ei), eQc(pi, ei)) * (-1 / (eV(pi, ei) * eV(pi, ei)));

                F[iePc(pi, ei)] = ePc(pi, ei) - ePf(pi, ei) + dPfc(eQf(pi, ei), eQc(pi, ei));

                for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                    int si = _usedSolutes[sii];

                    /*if (si == ROSolutes::CO2) {
                        //Cp
                        J.coeffRef(iesCp(pi, ei, sii), iesCp(pi, ei, sii)) = 1;
                        F[iesCp(pi, ei, sii)] = esCp(pi, ei, sii) - syssCaf(sii); //- esCf(pi, ei, sii);

                        //Cc
                        J.coeffRef(iesCc(pi, ei, sii), iesCc(pi, ei, sii)) = 1;
                        F[iesCc(pi, ei, sii)] = esCc(pi, ei, sii) - syssCaf(sii); //- esCf(pi, ei, sii);
                    } else */

                    if (si == ROSolutes::CO3) {

                        //Cp
                        J.coeffRef(iesCp(pi, ei, sii), iesCp(pi, ei, sii)) = 1;
                        J.coeffRef(iesCp(pi, ei, sii), iesCp(pi, ei, hco3i)) = -dco3hco3(T, ePHp(pi, ei));
                        J.coeffRef(iesCp(pi, ei, sii), iePHp(pi, ei)) = -dco3ph(esCp(pi, ei, hco3i), T, ePHp(pi, ei));

                        F[iesCp(pi, ei, sii)] = esCp(pi, ei, sii) - co3(esCp(pi, ei, hco3i), T, ePHp(pi, ei));

                        //Cc
                        J.coeffRef(iesCc(pi, ei, sii), iesCc(pi, ei, sii)) = 1;
                        J.coeffRef(iesCc(pi, ei, sii), iesCc(pi, ei, hco3i)) = -dco3hco3(T, ePHc(pi, ei));
                        J.coeffRef(iesCc(pi, ei, sii), iePHc(pi, ei)) = -dco3ph(esCc(pi, ei, hco3i), T, ePHc(pi, ei));
                        F[iesCc(pi, ei, sii)] = esCc(pi, ei, sii) - co3(esCc(pi, ei, hco3i), T, ePHc(pi, ei));
                    } else {

                        //SPm
                        J.coeffRef(iesSPm(pi, ei, sii), iesSPm(pi, ei, sii)) = 1;
                        if (si == ROSolutes::B) {
                            J.coeffRef(iesSPm(pi, ei, sii), iePHf(pi, ei)) = - pSPI[pi] * dSPmPh(MSi(pi, ei), ePHf(pi, ei));
                            F[iesSPm(pi, ei, sii)] = esSPm(pi, ei, sii) - pSPI[pi] * SPmPh(MSi(pi, ei), ePHf(pi, ei));
                        } else {
                            J.coeffRef(iesSPm(pi, ei, sii), iesCf(pi, ei, sii)) = - pSPI[pi] * dSPm(MSi(pi, ei), si, esCf(pi, ei, sii));
                            F[iesSPm(pi, ei, sii)] = esSPm(pi, ei, sii) - pSPI[pi] * SPm(MSi(pi, ei), si, esCf(pi, ei, sii));
                        }

                        //Cp
                        if (ei > 0) {
                            J.coeffRef(iesCp(pi, ei, sii), ievQf(pi, ei)) = -esCf(pi, ei, sii) * dKpQf(eQp(pi, ei), eQf(pi, ei), esSPm(pi, ei, sii)) * (-1 / (eV(pi, ei) * eV(pi, ei)));
                        }
                        J.coeffRef(iesCp(pi, ei, sii), ieQp(pi, ei)) = -esCf(pi, ei, sii) * dKpQp(eQp(pi, ei), eQf(pi, ei), esSPm(pi, ei, sii));
                        J.coeffRef(iesCp(pi, ei, sii), iesCp(pi, ei, sii)) = 1;
                        J.coeffRef(iesCp(pi, ei, sii), iesCf(pi, ei, sii)) = -Kp(eQp(pi, ei), eQf(pi, ei), esSPm(pi, ei, sii));
                        J.coeffRef(iesCp(pi, ei, sii), iesSPm(pi, ei, sii)) = -esCf(pi, ei, sii) * dKpSPm(eQp(pi, ei), eQf(pi, ei), esSPm(pi, ei, sii));
                        F[iesCp(pi, ei, sii)] = esCp(pi, ei, sii) - Kp(eQp(pi, ei), eQf(pi, ei), esSPm(pi, ei, sii)) * esCf(pi, ei, sii);

                        //Cc
                        J.coeffRef(iesCc(pi, ei, sii), iesCc(pi, ei, sii)) = eQc(pi, ei);
                        J.coeffRef(iesCc(pi, ei, sii), iesCp(pi, ei, sii)) = eQp(pi, ei);
                        J.coeffRef(iesCc(pi, ei, sii), ieQp(pi, ei)) = esCp(pi, ei, sii);
                        J.coeffRef(iesCc(pi, ei, sii), iesCf(pi, ei, sii)) = -eQf(pi, ei);

                        if (ei > 0) {
                            J.coeffRef(iesCc(pi, ei, sii), ievQf(pi, ei)) = -esCf(pi, ei, sii) * (-1 / (eV(pi, ei) * eV(pi, ei)));
                        } else if (ei < LAST_ei(pi)) {
                            J.coeffRef(iesCc(pi, ei, sii), ievQc(pi, ei)) = esCc(pi, ei, sii) * (-1 / (eV(pi, ei) * eV(pi, ei)));
                        }

                        F[iesCc(pi, ei, sii)] = eQp(pi, ei) * esCp(pi, ei, sii) + eQc(pi, ei) * esCc(pi, ei, sii) - eQf(pi, ei) * esCf(pi, ei, sii);
                    }

                    if (si != ROSolutes::CO2) {
                        // on element, not solute
                        J.coeffRef(ieIp(pi, ei), iesCp(pi, ei, sii)) = -_preComputedICoeffs(sii);
                        F[ieIp(pi, ei)] -= esCp(pi, ei, sii) * _preComputedICoeffs(sii);

                        J.coeffRef(ieIc(pi, ei), iesCc(pi, ei, sii)) = -_preComputedICoeffs(sii);
                        F[ieIc(pi, ei)] -= esCc(pi, ei, sii) * _preComputedICoeffs(sii);

                        J.coeffRef(ieCc(pi, ei), iesCc(pi, ei, sii)) = -1;
                        F[ieCc(pi, ei)] -= esCc(pi, ei, sii);

                        J.coeffRef(ieCp(pi, ei), iesCp(pi, ei, sii)) = -1;
                        F[ieCp(pi, ei)] -= esCp(pi, ei, sii);
                    }


                }
            }


            // pPHf
            F[ipPHf(pi)] = pPHf(pi);
            J.coeffRef(ipPHf(pi), ipPHf(pi)) = 1;
            if (hco3i >= 0) {
                J.coeffRef(ipPHf(pi), ipsCf(pi, hco3i)) = -dphhco3(psCf(pi, hco3i));
                J.coeffRef(ipPHf(pi), ipsCf(pi, co2i)) = -dphco2(psCf(pi, co2i));
                J.coeffRef(ipPHf(pi), ipIf(pi)) = -dphi(pIf(pi));
                F[ipPHf(pi)] = pPHf(pi) - ph(psCf(pi, hco3i), psCf(pi, co2i), T, pIf(pi));

            } else {
                F[ipPHf(pi)] = pPHf(pi) - sysPHaf; //ePHf(pi, ei);
            }

//            qDebug() << "pPHf" << pPHf(pi) << psCf(pi, hco3i) << psCf(pi, co2i) << pIf(pi) << k1(T) << ph(psCf(pi, hco3i), psCf(pi, co2i), T, pIf(pi));

            // s1PHf
            F[is1PHf(pi)] = s1PHf(pi);
            J.coeffRef(is1PHf(pi), is1PHf(pi)) = 1;
            if (hco3i >= 0) {
                J.coeffRef(is1PHf(pi), is1sCf(pi, hco3i)) = -dphhco3(s1sCf(pi, hco3i));
                J.coeffRef(is1PHf(pi), is1sCf(pi, co2i)) = -dphco2(s1sCf(pi, co2i));
                J.coeffRef(is1PHf(pi), is1If(pi)) = -dphi(s1If(pi));
                F[is1PHf(pi)] = s1PHf(pi) - ph(s1sCf(pi, hco3i), s1sCf(pi, co2i), T, s1If(pi));
            } else {
                F[is1PHf(pi)] = s1PHf(pi) - sysPHaf; //ePHf(pi, ei);
            }

            // pPHts
            F[ipPHp(pi)] = pPHp(pi);
            J.coeffRef(ipPHp(pi), ipPHp(pi)) = 1;
            if (hco3i >= 0) {
                J.coeffRef(ipPHp(pi), ipsCp(pi, hco3i)) = -dphhco3(psCp(pi, hco3i));
                J.coeffRef(ipPHp(pi), ipsCp(pi, co2i)) = -dphco2(psCp(pi, co2i));
                J.coeffRef(ipPHp(pi), ipIp(pi)) = -dphi(pIp(pi));
                F[ipPHp(pi)] = pPHp(pi) - ph(psCp(pi, hco3i), psCp(pi, co2i), T, pIp(pi));
            } else {
                F[ipPHp(pi)] = pPHp(pi) - sysPHaf; //ePHf(pi, ei);
            }

            // pPHp
            F[ipPHpb(pi)] = pPHpb(pi);
            J.coeffRef(ipPHpb(pi), ipPHpb(pi)) = 1;
            if (hco3i >= 0) {
                J.coeffRef(ipPHpb(pi), ipsCpb(pi, hco3i)) = -dphhco3(psCpb(pi, hco3i));
                J.coeffRef(ipPHpb(pi), ipsCpb(pi, co2i)) = -dphco2(psCpb(pi, co2i));
                J.coeffRef(ipPHpb(pi), ipIpb(pi)) = -dphi(pIpb(pi));
                F[ipPHpb(pi)] = pPHpb(pi) - ph(psCpb(pi, hco3i), psCpb(pi, co2i), T, pIpb(pi));
            } else {
                F[ipPHpb(pi)] = pPHpb(pi) - sysPHaf; //ePHf(pi, ei);
            }

            // I
            F[ipIf(pi)] = pIf(pi);
            F[is1If(pi)] = s1If(pi);
            F[ipIp(pi)] = pIp(pi);
            F[ipIpb(pi)] = pIpb(pi);

            J.coeffRef(ipIf(pi), ipIf(pi)) = 1;
            J.coeffRef(is1If(pi), is1If(pi)) = 1;
            J.coeffRef(ipIp(pi), ipIp(pi)) = 1;
            J.coeffRef(ipIpb(pi), ipIpb(pi)) = 1;

            // C
            F[ipCf(pi)] = -pCf(pi);
            F[is1Cf(pi)] = -s1Cf(pi);
            F[ipCp(pi)] = -pCp(pi);
            F[ipCpb(pi)] = -pCpb(pi);

            J.coeffRef(ipCf(pi), ipCf(pi)) = -1;
            J.coeffRef(is1Cf(pi), is1Cf(pi)) = -1;
            J.coeffRef(ipCp(pi), ipCp(pi)) = -1;
            J.coeffRef(ipCpb(pi), ipCpb(pi)) = -1;

            // PI
            QMap<int, double> s1fSolutes;

            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                s1fSolutes[si] = s1sCf(pi, sii);
            }
            J.coeffRef(is1PIf(pi), is1PIf(pi)) = 1;
            J.coeffRef(is1PIf(pi), is1Cf(pi)) = dPI_oldTds(T);
            F[is1PIf(pi)] = s1PIf(pi) - PI_old(s1Cf(pi), T);
//            F[is1PIf(pi)] = s1PIf(pi) - PI(s1fSolutes, T);

            // Pf
            J.coeffRef(is1Pf(pi), is1Pf(pi)) = 1;
            // Qf, Qp, S, T, pFF - const
            J.coeffRef(is1Pf(pi), ie1vQc(pi)) = -dPfQc(e1Qf(pi), e1Qc(pi)) * (-1 / (e1V(pi) * e1V(pi)));
            J.coeffRef(is1Pf(pi), ieQp(pi, 0)) = -dPfQp(e1Qf(pi), e1Qp(pi), s1Cf(pi), s1Cc(pi), s1PIf(pi), e1S(pi), T, pFF(pi), MSi(pi, 0));
            J.coeffRef(is1Pf(pi), is1Cf(pi)) = -dPfCf(e1Qf(pi), e1Qp(pi), s1Cf(pi), s1Cc(pi), s1PIf(pi), e1S(pi), T, pFF(pi), MSi(pi, 0));
            J.coeffRef(is1Pf(pi), is1Cc(pi)) = -dPfCc(e1Qf(pi), e1Qp(pi), s1Cf(pi), s1Cc(pi), s1PIf(pi), e1S(pi), T, pFF(pi), MSi(pi, 0));
            J.coeffRef(is1Pf(pi), is1PIf(pi)) = -dPfPIf();
            J.coeffRef(is1Pf(pi), is1PIp(pi)) = -dPfPIp();
            J.coeffRef(is1Pf(pi), is1PIc(pi)) = -dPfPIc();
            F[is1Pf(pi)] = s1Pf(pi) - Pf(e1Qf(pi), e1Qp(pi), e1Qc(pi), s1Cf(pi), s1Cc(pi),
                                         s1PIf(pi), s1PIp(pi), s1PIc(pi), e1S(pi), T, pFF(pi), MSi(pi, 0));

            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];

/*                if (si == ROSolutes::CO2) {
                    // psCfr
                    J.coeffRef(ipsCfr(pi, sii), ipsCfr(pi, sii)) = 1;
                    F[ipsCfr(pi, sii)] = psCfr(pi, sii) - syssCaf(sii); //esCf(pi, sii);

                    // s1sCf
                    J.coeffRef(is1sCf(pi, sii), is1sCf(pi, sii)) = 1;
                    F[is1sCf(pi, sii)] = s1sCf(pi, sii) - syssCaf(sii); //esCf(pi, sii);

                    // psCts
                    J.coeffRef(ipsCts(pi, sii), ipsCts(pi, sii)) = 1;
                    F[ipsCts(pi, sii)] = psCts(pi, sii) - syssCaf(sii); //esCf(pi, sii);

                    // psCp
                    J.coeffRef(ipsCp(pi, sii), ipsCp(pi, sii)) = 1;
                    F[ipsCp(pi, sii)] = psCp(pi, sii) - syssCaf(sii); //esCf(pi, sii);

                } else *//*if (si == ROSolutes::CO3) {

                    // psCfr
                    J.coeffRef(ipsCf(pi, sii), ipsCf(pi, sii)) = 1;
                    J.coeffRef(ipsCf(pi, sii), ipsCf(pi, hco3i)) = -dco3hco3(T, pPHf(pi));
                    J.coeffRef(ipsCf(pi, sii), ipPHf(pi)) = -dco3ph(psCf(pi, hco3i), T, pPHf(pi));
                    F[ipsCf(pi, sii)] = psCf(pi, sii) - co3(psCf(pi, hco3i), T, pPHf(pi));

                    // s1sCf
                    J.coeffRef(is1sCf(pi, sii), is1sCf(pi, sii)) = 1;
                    J.coeffRef(is1sCf(pi, sii), is1sCf(pi, hco3i)) = -dco3hco3(T, s1PHf(pi));
                    J.coeffRef(is1sCf(pi, sii), is1PHf(pi)) = -dco3ph(s1sCf(pi, hco3i), T, s1PHf(pi));
                    F[is1sCf(pi, sii)] = s1sCf(pi, sii) - co3(s1sCf(pi, hco3i), T, s1PHf(pi));

                    // psCp
                    J.coeffRef(ipsCp(pi, sii), ipsCp(pi, sii)) = 1;
                    J.coeffRef(ipsCp(pi, sii), ipsCp(pi, hco3i)) = -dco3hco3(T, pPHp(pi));
                    J.coeffRef(ipsCp(pi, sii), ipPHp(pi)) = -dco3ph(psCp(pi, hco3i), T, pPHp(pi));
                    F[ipsCp(pi, sii)] = psCp(pi, sii) - co3(psCp(pi, hco3i), T, pPHp(pi));

                    // psCpb
                    J.coeffRef(ipsCpb(pi, sii), ipsCpb(pi, sii)) = 1;
                    J.coeffRef(ipsCpb(pi, sii), ipsCpb(pi, hco3i)) = -dco3hco3(T, pPHpb(pi));
                    J.coeffRef(ipsCpb(pi, sii), ipPHpb(pi)) = -dco3ph(psCpb(pi, hco3i), T, pPHpb(pi));
                    F[ipsCpb(pi, sii)] = psCpb(pi, sii) - co3(psCpb(pi, hco3i), T, pPHpb(pi));
                } else {*/
                    // pass Cfr
                    J.coeffRef(ipsCf(pi, sii), ipsCf(pi, sii)) = -pQf(pi);
                    if(pi>0)
                        J.coeffRef(ipsCf(pi, sii), ipsCpb(pi-1, sii)) = pQfb(pi);
                    F[ipsCf(pi, sii)] = pQfb(pi) * psCraw(pi, sii) - psCf(pi, sii) * pQf(pi);

                    auto passInRecycles = pass->incomingRecycles(); // TODO move to Eigen vector
                    auto ri = passInRecycles.constBegin();
                    while (ri != passInRecycles.constEnd()) {
                        int rpi = ri.key(); // recycle pi
                        J.coeffRef(ipsCf(pi, sii), iesCc(rpi, LAST_ei(rpi), sii)) = ri.value();
                        F[ipsCf(pi, sii)] += esCc(rpi, LAST_ei(rpi), sii) * ri.value(); // CR*QR
                        ++ri;
                    }


                    J.coeffRef(ipCf(pi), ipsCf(pi, sii)) = 1;
                    F[ipCf(pi)] += psCf(pi, sii);


                    // pass stage 1 Cf
                    J.coeffRef(is1sCf(pi, sii), is1sCf(pi, sii)) = -e1vQf(pi);
                    J.coeffRef(is1sCf(pi, sii), ipsCf(pi, sii)) = pQf(pi);
                    J.coeffRef(is1sCf(pi, sii), iesCc(pi, LAST_ei(pi), sii)) = pQsr(pi);
                    F[is1sCf(pi, sii)] = pQf(pi)*psCf(pi, sii) + pQsr(pi)*esCc(pi, LAST_ei(pi), sii) - e1vQf(pi) * s1sCf(pi, sii);

                    J.coeffRef(is1Cf(pi), is1sCf(pi, sii)) = 1;
                    F[is1Cf(pi)] += s1sCf(pi, sii);

                    // pass Cp
                    J.coeffRef(ipsCp(pi, sii), ipsCp(pi, sii)) = -pQp(pi);
                    F[ipsCp(pi, sii)] = -pQp(pi) * psCp(pi, sii);
                    for(int ei2 = 0; ei2 < peCount(pi); ++ei2) {
                        J.coeffRef(ipsCp(pi, sii), iesCp(pi, ei2, sii)) = evQp(pi, ei2);
                        J.coeffRef(ipsCp(pi, sii), ieQp(pi, ei2)) = esCp(pi, ei2, sii) * eV(pi, ei2);
                        F[ipsCp(pi, sii)] += evQp(pi, ei2) * esCp(pi, ei2, sii); // Qp и Qc эл-та по смещению
                    }

                    J.coeffRef(ipCp(pi), ipsCp(pi, sii)) = 1;
                    F[ipCp(pi)] += psCp(pi, sii);


                    // pass Cpb
                    J.coeffRef(ipsCpb(pi, sii), ipsCpb(pi, sii)) = -pQpb(pi);
                    J.coeffRef(ipsCpb(pi, sii), ipsCp(pi, sii)) = pQp(pi);
                    if(pi>0)
                        J.coeffRef(ipsCpb(pi, sii), ipsCpb(pi-1, sii)) = pQb[pi];
                    F[ipsCpb(pi, sii)] = pQp(pi) * psCp(pi, sii) + pQb(pi) * psCraw(pi, sii) - pQpb(pi) * psCpb(pi, sii);

                    J.coeffRef(ipCpb(pi), ipsCpb(pi, sii)) = 1;
                    F[ipCpb(pi)] += psCpb(pi, sii);
//                }

                if (si != ROSolutes::CO2) {
                    // pIfr
                    J.coeffRef(ipIf(pi), ipsCf(pi, sii)) = -_preComputedICoeffs(sii);
                    F[ipIf(pi)] -= psCf(pi, sii) * _preComputedICoeffs(sii);

                    // s1If
                    J.coeffRef(is1If(pi), is1sCf(pi, sii)) = -_preComputedICoeffs(sii);
                    F[is1If(pi)] -= s1sCf(pi, sii) * _preComputedICoeffs(sii);

                    // pIp
                    J.coeffRef(ipIp(pi), ipsCp(pi, sii)) = -_preComputedICoeffs(sii);
                    F[ipIp(pi)] -= psCp(pi, sii) * _preComputedICoeffs(sii);

                    // pIpb
                    J.coeffRef(ipIpb(pi), ipsCpb(pi, sii)) = -_preComputedICoeffs(sii);
                    F[ipIpb(pi)] -= psCpb(pi, sii) * _preComputedICoeffs(sii);
                }
            }
        }

        elapsed += t.nsecsElapsed();

        SET_MATRIX_TIME += timer.nsecsElapsed();
        timer.restart();



        if (stepCntr == 0)
            J.makeCompressed();

        // SOLVE IT!
        _solver.compute(J);
        dX = _solver.solve(F);
        bool solution_exists = (J*dX).isApprox(F, _tolerance);

//#ifdef QT_DEBUG
//        qDebug() << "stop solving system";
//#endif

        SOLVE_MATRIX_TIME += timer.nsecsElapsed();
        ++CALC_MATRIX_COUNT;


        X.noalias() -= dX; //noalias - non copy optimization



        if (!solution_exists) {

#ifdef QT_DEBUG
            myfile << "X:" << std::endl << X.transpose() << std::endl;
            myfile << "dX:" << std::endl << dX.transpose() << std::endl;
            myfile << "F:" << std::endl << F.transpose() << std::endl;
            myfile << "J:" << std::endl << J << std::endl;
            myfile.flush();
//            addLogV();
#endif
            return false;
        }

        solved = ((X.array() > -tolerance()).all() && (dX.array().abs()/*.mean()*/ < _tolerance).all());

        // CORRECTION
        //X = X.array().abs();



#ifdef QT_DEBUG
        qDebug() << "COUNTER:" << stepCntr << solved /*<< relError << solution_exists*/ << dX.array().abs().maxCoeff() << dX.array().abs().mean() << _tolerance;
        if (!solved) {
            if (!solution_exists) qDebug() << "NOT SOLVED BECAUSE !solution_exists";
            if (!(X.array() > 0.0).all()) qDebug() << "NOT SOLVED BECAUSE !(X.array() > 0.0).all()";
            if (!(dX.array().abs() < _tolerance).all()) qDebug() << "NOT SOLVED BECAUSE !(dX.array().abs() < _tolerance).all()";
        }
#endif

        ++stepCntr;

    }

//        logValues();

#ifdef QT_DEBUG

        myfile.close();
#endif

    return solved;
}

void ROSystemSolver::initSystem() {
    // INIT
    for (int pi=0; pi<_sys->passCount(); ++pi) {
        initPass(pi);
    }
}

void ROSystemSolver::logValues() {
    for (int pi=0; pi<_sys->passCount(); ++pi) {
        qDebug() << "pQp:" << pQp(pi);
        qDebug() << "pCfr:" << pCf(pi);
        qDebug() << "s1Cf:" << s1Cf(pi);
        qDebug() << "s1Qf:" << e1vQf(pi);
        qDebug() << "pCp:" << pCp(pi);
        qDebug() << "pCpb:" << pCpb(pi);

        qDebug() << "pPHf:" << pPHf(pi);
        qDebug() << "s1PHf:" << s1PHf(pi);
        qDebug() << "pPHp:" << pPHp(pi);
        qDebug() << "pPHpb:" << pPHpb(pi);

        qDebug() << "pIfr:" << pIf(pi);
        qDebug() << "s1If:" << s1If(pi);
        qDebug() << "pIp:" << pIp(pi);
        qDebug() << "pIpb:" << pIpb(pi);
        for(int ei2 = 0; ei2 < peCount(pi); ++ei2) {
            //qDebug() << "e" << ei2 << "  Qp:" << eQp(pi, ei2);
            qDebug() << "e" << ei2 << "  Qp:" << evQp(pi, ei2);
            //qDebug() << "e" << ei2 << "  Qp3:" << Qp(ePf(pi, ei2), eQf(pi, ei2), eQc(pi, ei2), eCf(pi, ei2), eCc(pi, ei2), ePIf(pi, ei2), ePIp(pi, ei2), ePIc(pi, ei2), eS(pi, ei2), T, pFF(pi));
            qDebug() << "e" << ei2 << "  Cp:" << eCp(pi, ei2);
            qDebug() << "e" << ei2 << "  Qc:" << evQc(pi, ei2);
            qDebug() << "e" << ei2 << "  Cc:" << eCc(pi, ei2);
            qDebug() << "e" << ei2 << "  PHp:" << ePHp(pi, ei2);
            qDebug() << "e" << ei2 << "  PHc:" << ePHc(pi, ei2);
            qDebug() << "e" << ei2 << "  Ip:" << eIp(pi, ei2);
            qDebug() << "e" << ei2 << "  Ic:" << eIc(pi, ei2);
            qDebug() << "e" << ei2 << "  PI[f,p,c]:" << ePIf(pi, ei2) << ePIp(pi, ei2) << ePIc(pi, ei2);
            qDebug() << "e" << ei2 << "  Pf:" << ePf(pi, ei2);
            qDebug() << "e" << ei2 << "  Pc:" << ePc(pi, ei2);
            for (int sii=0; sii<_usedSolutes.count(); ++sii) {
                int si = _usedSolutes[sii];
                if (si != ROSolutes::CO2 && si != ROSolutes::CO3 && si != ROSolutes::HCO3)
                    continue;

                qDebug() << "    sCp" << si << esCp(pi, ei2, sii);
                qDebug() << "    sCc" << si << esCc(pi, ei2, sii);
                qDebug() << "    sSPm" << si << esSPm(pi, ei2, sii);
            }
        }

    }
}


void ROSystemSolver::setTolerance(double value) {
    _tolerance = qMax(0.0001, value);
    Q_EMIT toleranceChanged();
}
