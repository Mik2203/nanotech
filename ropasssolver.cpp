#include "ropasssolver.h"

#define _USE_MATH_DEFINES // for MSVC 2010 C++
#include <math.h>
#include "romath.h"
#include <Eigen/Dense>

#ifdef QT_DEBUG
#include <iostream>
#include <QDebug>
#endif


#include "rosystem.h"

ROPassSolver::ROPassSolver(QObject *parent) :
    QObject(parent)
{
}


double ROPassSolver::_tolerance = 10e-5;


double ROPassSolver::tolerance() { return _tolerance; }
void ROPassSolver::setTolerance(double value) { _tolerance = value; }

bool ROPassSolver::solve(ROPass* const pass) {

    // TODO proverka na 4itaemost'
    double T = pass->system()->temperature();
    double pQf = pass->feed()->rate();
    double pCf = pass->feed()->solutes()->totalValue(ROSolutes::Meql);
    double pQp = pass->permeate()->rate();
    double pQr = pass->selfRecycle();
    double pQfr = pQf + pQr;
    double FF = pass->flowFactor();

    // Подсчет количества элементов в ступени
    int elsCount = 0;
    for(int stageIndex = 0; stageIndex < pass->stageCount(); ++stageIndex) {
        ROStage* stage = pass->stage(stageIndex);
        elsCount += stage->elementsPerVesselCount();
    }

    // Инициализация переменных c данными (SPm, S)
    Eigen::VectorXd SPm(elsCount);
    Eigen::VectorXd s(elsCount);
    //Eigen::VectorXd PB = Eigen::VectorXd::Zero(elsCount);
    Eigen::VectorXd Pb = Eigen::VectorXd::Zero(elsCount); // Back pressure
    for(int stageIndex = 0, elIndex = 0; stageIndex < pass->stageCount(); ++stageIndex) {
        ROStage* stage = pass->stage(stageIndex);
        SPm.segment(elIndex, stage->elementsPerVesselCount()) = Eigen::VectorXd::Constant(stage->elementsPerVesselCount(), 1 - stage->membrane()->rejection() / 100.0);
        s.segment(elIndex, stage->elementsPerVesselCount()) = Eigen::VectorXd::Constant(stage->elementsPerVesselCount(), stage->membrane()->activeArea() * stage->vesselCount());
        Pb[elIndex] = stage->preStagePressure();
        //qDebug() << elIndex << ":" << Pb[elIndex];
        elIndex += stage->elementsPerVesselCount();

        //PB[elIndex-1] = stage->permeate()->pressure();
    }

//    std::cout << SPm;
//    std::cout << s;
//    std::cout << PB;

    // Инициализация
    // 5 значений для вычислений (Cp, Cc, Pf, Qc, Qp) в каждом элементе
    // +1 на Cfr
    int size = elsCount * 5 + 1;
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(size,size); // Якобиан
    Eigen::VectorXd X(size); // Вектор переменных
    Eigen::VectorXd dX(size); // Вектор приращений на каждом шаге
    Eigen::VectorXd F(size); // Вектор значений функций

#define iCP off
#define iCC off+1
#define iPF off+2
#define iQC off+3
#define iQP off+4
#define iCFR size-1

#define iL_CC size-5
#define iP_CC off-4
#define iP_P off-3
#define iP_QC off-2

#define CP X[off]
#define CC X[iCC]
#define PF X[iPF]
#define QC X[iQC]
#define QP X[iQP]
#define CFR X[size-1]

#define P_QC X[iP_QC] // previous CC
#define P_CC X[iP_CC] // previous CC
#define L_CC X[iL_CC] // last CC
#define P_P X[iP_P]   // previous P

#define SPM SPm[eI]
#define S s[eI]
#define PB Pb[eI]
#define P_PB Pb[eI-1]


    int eI = 0; // element index
    int off = 0;

    // Инициализация вектора переменных
    for(eI = 0; eI < elsCount; ++eI) {
        off = eI * 5;
        QP = pQp / elsCount;
        QC = pQf / elsCount;
        CP = pCf * 0.1;
        CC = pCf * 2.0;
        if (eI == 0)
            PF = 8 * (50 * QP + 1.5 * pQfr) / S;
        else
            PF = 8 * (50 * QP + 1.5 * P_QC) / S;
    }
    CFR = (pQf * pCf + pQr * L_CC) / pQfr;
//#ifdef QT_DEBUG
    //std::cout << "init X:" << X << std::endl;
//#endif
    bool solved = false;
    int stepCntr = 0;
    while (!solved && stepCntr < 100) {
        // First element data
        eI = 0;
        off = 0;

        J(iCP, iQP) = -CFR*dKpQp(QP, pQfr, SPM);
        J(iCP, iCP) = 1;
        J(iCP, iCFR) = -Kp(QP, pQfr, SPM);

        J(iCC, iQP) = -CFR*dKcQp(QP, pQfr, SPM);
        J(iCC, iCC) = 1;
        J(iCC, iCFR) = -Kc(QP, pQfr, SPM);

        J(iPF, iQP) = -dPQp(QP, pQfr, QC, CFR, CC, SPM, S, T, FF);
        J(iPF, iQC) = -dPQc(QP, pQfr, QC, CFR, CC, SPM, S, T, FF);
        J(iPF, iCC) = -dPCc(QP, pQfr, QC, CFR, CC, SPM, S, T, FF);
        J(iPF, iPF) = 1;
        J(iPF, iCFR) = -dPCf(QP, pQfr, QC, CFR, CC, SPM, S, T, FF);

        J(iQC, iQP) = CP;
        J(iQC, iQC) = CC;
        J(iQC, iCP) = QP;
        J(iQC, iCC) = QC;
        J(iQC, iCFR) = -pQfr;

        for(int elIdx = 0; elIdx < elsCount; ++elIdx) {
            J(iQP, elIdx * 5 + 4) = 1; // Для всех QP
        }

        J(iCFR, iL_CC) = pQr; // last element Cc
        J(iCFR, iCFR) = -pQfr; // first element Cf

        F[iCP] = CP - Kp(QP, pQfr, SPM)*CFR;
        F[iCC] = CC - Kc(QP, pQfr, SPM)*CFR;
        F[iPF] = PF - (P(QP, pQfr, QC, CFR, CC, SPM, PB, S, T, FF) + PB);
        F[iQC] = QP*CP + QC*CC - pQfr*CFR;
        F[iQP] = -pQp;
        F[iCFR] = pQf*pCf + pQr*L_CC - pQfr*CFR;
        for(int elIdx = 0; elIdx < elsCount; ++elIdx)
            F[iQP] += X[elIdx * 5 + 4]; // Для всех QP


        // Other elements data
        for(eI = 1; eI < elsCount; ++eI) {
            off = eI * 5;
            J(iCP, iQP) = -P_CC*dKpQp(QP, P_QC, SPM);
            J(iCP, iCP) = 1;
            J(iCP, iP_CC) = -Kp(QP, P_QC, SPM);
            J(iCP, iP_QC) = -P_CC*dKpQf(QP, P_QC, SPM);

            J(iCC, iQP) = -P_CC*dKcQp(QP, P_QC, SPM);
            J(iCC, iCC) = 1;
            J(iCC, iP_CC) = -Kc(QP, P_QC, SPM);
            J(iCC, iP_QC) = -P_CC*dKcQf(QP, P_QC, SPM);

            J(iPF, iPF) = -1;
            J(iPF, iP_P) = 1;

            J(iQC, iQP) = CP;
            J(iQC, iQC) = CC;
            J(iQC, iCP) = QP;
            J(iQC, iCC) = QC;
            J(iQC, iP_CC) = -P_QC;
            J(iQC, iP_QC) = -P_CC;

            J(iQP, iQP) = -dPQp(QP, P_QC, QC, P_CC, CC, SPM, PB, S, T, FF);
            J(iQP, iQC) = -dPQc(QP, P_QC, QC, P_CC, CC, SPM, PB, S, T, FF);
            J(iQP, iCC) = -dPCc(QP, P_QC, QC, P_CC, CC, SPM, PB, S, T, FF);
            J(iQP, iP_CC) = -dPCf(QP, P_QC, QC, P_CC, CC, SPM, PB, S, T, FF);
            J(iQP, iP_QC) = -dPQf(QP, P_QC, QC, P_CC, CC, SPM, PB, S, T, FF);
            J(iQP, iPF) = 1;

            F[iCP] = CP - Kp(QP, P_QC, SPM)*P_CC;
            F[iCC] = CC - Kc(QP, P_QC, SPM)*P_CC;
            F[iPF] = P_P - (PF + P_PB);
            F[iQC] = QP*CP + QC*CC - P_QC*P_CC;
            F[iQP] = PF - (P(QP, P_QC, QC, P_CC, CC, SPM, PB, S, T, FF) + PB);
        }

        //std::cout << J << std::endl;
        //std::cout << F << std::endl;
        dX = J.colPivHouseholderQr().solve(F); // J.inverse() * F
        bool solution_exists = (J*dX).isApprox(F, _tolerance);
        if (!solution_exists) break; // solution is unreachable or bad method used.

        //X2.noalias() = X - dX; // J.inverse() * F
        //Eigen::ArrayXd dX = X2-X;
        //std::cout << dX << std::endl;
        X.noalias() -= dX; //noalias - non copy optimization
        if ((dX.array().abs() < _tolerance).all()) {
            // TODO nado li proveryat' X ?
            solved = true;
//            std::cout << "X:" << X << std::endl;
//            std::cout << "CNTR:" << stepCntr << std::endl;
        }
        else {
            ++stepCntr;
        }
    }

    if (solved) {
        eI = 0;
        ROStage* firstStage = pass->firstStage();
        firstStage->feed()->setRate(pQfr);
        firstStage->feed()->copySolutesFrom(pass->feed());
        firstStage->feed()->solutes()->setTotalValue(CFR, ROSolutes::Meql);


        double pCPQP = 0.0;
        double sCPQP = 0.0;
        double sQP = 0.0;
        //double pCPQP = 0.0;
        for(int stageIndex = 0; stageIndex < pass->stageCount(); ++stageIndex) {

            ROStage* stage = pass->stage(stageIndex);
            sCPQP = 0.0;
            sQP = 0.0;

            ROElement* firstElement = stage->firstElement();
            firstElement->feed()->setRate(stage->feed()->rate() / stage->vesselCount());
            firstElement->feed()->copySolutesFrom(stage->feed());

            for (int vIdx = 0; vIdx < stage->elementsPerVesselCount(); ++vIdx) {
                off = eI * 5;
                sCPQP += CP * QP;
                sQP += QP;

                stage->element(vIdx)->permeate()->copySolutesFrom(stage->feed());
                stage->element(vIdx)->permeate()->solutes()->setTotalValue(CP, ROSolutes::Meql);
                stage->element(vIdx)->concentrate()->copySolutesFrom(stage->feed());
                stage->element(vIdx)->concentrate()->solutes()->setTotalValue(CC, ROSolutes::Meql);

                stage->element(vIdx)->concentrate()->setRate(QC / stage->vesselCount());
                stage->element(vIdx)->permeate()->setRate(QP / stage->vesselCount());

                stage->element(vIdx)->feed()->setPressure(PF / stage->vesselCount());

                //stage->element(vIdx)->feed()->updateOsmoticPressure(PI_new(stage->element(vIdx)->feed()->solutes(), T));
                //stage->element(vIdx)->permeate()->updateOsmoticPressure(PI_new(stage->element(vIdx)->permeate()->solutes(), T));
                //stage->element(vIdx)->concentrate()->updateOsmoticPressure(PI_new(stage->element(vIdx)->concentrate()->solutes(), T));
                ++eI;
            }

            // Solutes
            stage->permeate()->copySolutesFrom(stage->feed());
            stage->concentrate()->copySolutesFrom(stage->lastElement()->concentrate());
            stage->permeate()->solutes()->setTotalValue(sCPQP / sQP, ROSolutes::Meql);

            //stage->concentrate()->copyDataFrom(stage->lastElement()->concentrate());// VOT ONO!!!!!!!!!!!!!!!!!!!!!!!!

            // Rates
            stage->concentrate()->setRate(stage->lastElement()->concentrate()->rate() * stage->vesselCount());
            stage->permeate()->setRate(sQP);

            // Pressures
            stage->feed()->setPressure(PF);
            //stage->concentrate()->setPressure(stage->lastElement());

            // Osm pressures
            //stage->feed()->updateOsmoticPressure(PI_new(stage->feed()->solutes(), T));
            //stage->permeate()->updateOsmoticPressure(PI_new(stage->permeate()->solutes(), T));
            //stage->concentrate()->updateOsmoticPressure(stage->lastElement()->concentrate()->osmoticPressure());

            pCPQP += sCPQP;

        }

        ROStage* lastStage = pass->lastStage();

        // pass solutes
        pass->concentrate()->copyDataFrom(lastStage->concentrate());
        pass->concentrate()->setRate(lastStage->concentrate()->rate() - pass->selfRecycle());
        //pass->permeate()->setRate(pQp);
        pass->permeate()->copySolutesFrom(pass->feed());
        pass->permeate()->solutes()->setTotalValue(pCPQP / pQp, ROSolutes::Meql);

        // pass pressures
        pass->feed()->setPressure(firstStage->feed()->pressure());
        pass->permeate()->setPressure(lastStage->permeate()->pressure());
        pass->concentrate()->setPressure(lastStage->concentrate()->pressure());

        // pass osm. pressures
        //pass->feed()->updateOsmoticPressure(firstStage->feed()->osmoticPressure());
        //pass->permeate()->updateOsmoticPressure(lastStage->permeate()->osmoticPressure());
        //pass->concentrate()->updateOsmoticPressure(lastStage->concentrate()->osmoticPressure());
    }


//    std::cout << "X:" << X << std::endl;
//    std::cout << "CNTR:" << stepCntr << std::endl;
    return solved;
}
