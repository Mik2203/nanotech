#ifndef ROSYSTEMSOLVER_H
#define ROSYSTEMSOLVER_H

#include "Eigen/Dense"
#include <Eigen/Sparse>
#include <QObject>
#include <QVector>

#include "rosystem.h"

/* ядро расчетов системы.
 * Содержит всю логику по формированию системы уравнений и ее расчету методом Ньютона.
 * Имеет параметр tolerance, задающий точность вычислений.
 */

class ROSystemSolver : public QObject {

    Q_OBJECT
    Q_PROPERTY(double tolerance READ tolerance WRITE setTolerance NOTIFY toleranceChanged)

public:
    explicit ROSystemSolver(ROSystem * const sys, QObject *parent = nullptr);
    ROSystemSolver() {} // FOR QML

    bool solved() const;
    double tolerance() const;
    void setTolerance(double value);

signals:
    void toleranceChanged();
    void finished();

public slots:
    void solve();



private:
    // ** ДЛЯ ОТЛАДКИ **
    QVector<QVector<double> > logV;
    QVector<QString> logT;
    void addLogV();
    void printLogV();
    void checkForNan(const Eigen::MatrixXd& mat);
    // **

//    enum Decomposition { PartialPivLU, ColPivHouseholderQR, FullPivHouseholderQR };
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> _solver;
    void setSystemValues();
    ROSystem * _sys;
    bool _setSystemValues;

    double _tolerance;

    bool _solved;

    QVector<int> _usedSolutes;
    int _elementEquationCount;
    int _passEquationCount;
//    Decomposition _decomposition;
    Eigen::MatrixXd _preComputedICoeffs;
    Eigen::VectorXd pQb;
    Eigen::VectorXd pQfb;
    Eigen::VectorXd pQraw;
    Eigen::VectorXd pQf;
    Eigen::VectorXd pQp;
    Eigen::VectorXd pQpb;
    Eigen::VectorXd pQsr;
    Eigen::VectorXd pQr;
    Eigen::VectorXd e1vQf;
    Eigen::VectorXd pFF;
    Eigen::VectorXd pSPI;
    Eigen::VectorXd s; // elements area
    Eigen::VectorXd v; // elements vessels count
    Eigen::VectorXd pp; // pre stage pressure
    Eigen::VectorXd ePb; // back pressure

    double sysCaf;
    double sysIaf;
    double sysPHaf;
    double sysQaf;
    double T;
    Eigen::VectorXd syssCaf;

    // matrix properties
    Eigen::VectorXi piCount;
    Eigen::VectorXi peCount;
    Eigen::VectorXi piOff;
    Eigen::VectorXi peOff;
    Eigen::VectorXi msi;
    Eigen::SparseMatrix<double> J;
    Eigen::VectorXd X;
    Eigen::VectorXd dX;
    Eigen::VectorXd F;

    bool init();

    void zeroMatrices();
    bool calcSystem();
    void initPass(int pi);
    void initSystem();
    void logValues();

    inline int ieOff(int pi, int ei) { return piOff(pi) + (ei) * _elementEquationCount; } // TODO PRECOMPUTE TO ARRAY
    inline int iesCp(int pi, int ei, int si) { return ieOff(pi, ei) + si; } // + _usedSolutes.count() - SPmi
    inline int ieCp(int pi, int ei) { return ieOff(pi, ei) + _usedSolutes.count(); }
    inline int iesCc(int pi, int ei, int si) { return ieOff(pi, ei) + _usedSolutes.count() + 1 + si; }
    inline int ieCc(int pi, int ei) { return ieOff(pi, ei) + 2 * _usedSolutes.count() + 1; }
    inline int ievQc(int pi, int ei) { return ieOff(pi, ei) + 2 * _usedSolutes.count() + 2; }
    inline int iesSPm(int pi, int ei, int si) { return ieOff(pi, ei) + 2 * _usedSolutes.count() + 3 + si; }
    inline int iePHp(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 3; }
    inline int iePHc(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 4; }
    inline int ieIp(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 5; }
    inline int ieIc(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 6; }
    inline int iePIp(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 7; }
    inline int iePIc(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 8; }
    inline int iePc(int pi, int ei) { return ieOff(pi, ei) + 3 * _usedSolutes.count() + 9; }
    inline int ieQp(int pi, int ei) { /*assert(ei > 0); */return ieOff(pi, ei) + 3 * _usedSolutes.count() + 10; } // ONLY FOR ei > 0
    inline int ievQf(int pi, int ei) { return ievQc(pi, ei-1); } // ONLY FOR ei > 0

    inline int ipOff(int pi) { return piOff(pi) + peCount(pi) * _elementEquationCount; }
    inline int ipsCf(int pi, int si) { return ipOff(pi) + si; }
    inline int ipCf(int pi) { return ipOff(pi) + _usedSolutes.count(); }
    inline int is1sCf(int pi, int si) { return ipOff(pi) + _usedSolutes.count() + 1 + si; }
    inline int is1Cf(int pi) { return ipOff(pi) + 2 * _usedSolutes.count() + 1; }
    inline int ie1vQc(int pi) { return ievQc(pi, 0); }
    inline int is1Cp(int pi) { return ieCp(pi, 0); }
    inline int is1Cc(int pi) { return ieCc(pi, 0); }
    inline int ipsCp(int pi, int si) { return ipOff(pi) + 2 * _usedSolutes.count() + 2 + si; }
    inline int ipCp(int pi) { return ipOff(pi) + 3 * _usedSolutes.count() + 2; }
    inline int ipsCpb(int pi, int si) { return ipOff(pi) + 3 * _usedSolutes.count() + 3 + si; }
    inline int ipCpb(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 3; }
    inline int ipPHf(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 4; }
    inline int is1PHf(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 5; }
    inline int ipPHp(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 6; }
    inline int ipPHpb(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 7; }
    inline int ipIf(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 8; }
    inline int is1If(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 9; }
    inline int is1PIf(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 10; }
    inline int is1PIp(int pi) { return iePIp(pi, 0); }
    inline int is1PIc(int pi) { return iePIc(pi, 0); }
    inline int is1Pf(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 11; }
    inline int ipIp(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 12; }
    inline int ipIpb(int pi) { return ipOff(pi) + 4 * _usedSolutes.count() + 13; }

    inline int iesCf(int pi, int ei, int si) { return ei == 0 ? is1sCf(pi, si) : iesCc(pi, ei-1, si); }      // only for ei > 0
    inline int ieCf(int pi, int ei) { return ei == 0 ? is1Cf(pi) : ieCc(pi, ei-1); }
    inline int ieIf(int pi, int ei) { return ei == 0 ? is1If(pi) : ieIc(pi, ei-1); }
    inline int iePIf(int pi, int ei) { return ei == 0 ? is1PIf(pi) : iePIc(pi, ei-1); }
    inline int iePf(int pi, int ei) { return ei == 0 ? is1Pf(pi) : iePc(pi, ei-1); }
    inline int iePHf(int pi, int ei) { return ei == 0 ? is1PHf(pi) : iePHc(pi, ei-1); }

    inline int LAST_ei(int pi) { return peCount(pi)-1; }
    inline int PRELAST_ei(int pi) {return peCount(pi)-2; }
    inline double& esCp(int pi, int ei, int si) { return X[iesCp(pi, ei, si)]; }
    inline double& eCp(int pi, int ei) { return X[ieCp(pi, ei)]; }
    inline double& esCc(int pi, int ei, int si) { return X[iesCc(pi, ei, si)]; }
    inline double& eCc(int pi, int ei) { return X[ieCc(pi, ei)]; }
    inline double& evQc(int pi, int ei) { return X[ievQc(pi, ei)]; }
    inline double eQc(int pi, int ei) { return evQc(pi, ei) / eV(pi, ei); } // by one vessel
    inline double& esSPm(int pi, int ei, int si) { return X[iesSPm(pi, ei, si)]; }
    inline double& ePHp(int pi, int ei) { return X[iePHp(pi, ei)]; }
    inline double& ePHc(int pi, int ei) { return X[iePHc(pi, ei)]; }
    inline double& eIp(int pi, int ei) { return X[ieIp(pi, ei)]; }
    inline double& eIc(int pi, int ei) { return X[ieIc(pi, ei)]; }
    inline double& ePIf(int pi, int ei) { return X[iePIf(pi, ei)]; }
    inline double& ePIp(int pi, int ei) { return X[iePIp(pi, ei)]; }
    inline double& ePIc(int pi, int ei) { return X[iePIc(pi, ei)]; }
    inline double& ePf(int pi, int ei) { return X[iePf(pi, ei)]; }
    inline double& ePc(int pi, int ei) { return X[iePc(pi, ei)]; }
    inline double& eQp(int pi, int ei) { return X[ieQp(pi, ei)]; }
    inline double evQp(int pi, int ei) { return eQp(pi, ei) * eV(pi, ei); }
    inline double& evQf(int pi, int ei) { return (ei == 0 ? e1vQf(pi) :  X[ievQf(pi, ei)]); }
    inline double eQf(int pi, int ei) { return evQf(pi, ei) / eV(pi, ei); } // by one vessel
    inline double& esCf(int pi, int ei, int si) { return X[iesCf(pi, ei, si)]; }
    inline double& eCf(int pi, int ei) { return X[ieCf(pi, ei)]; }
    inline double& ePHf(int pi, int ei) { return X[iePHf(pi, ei)]; }
    inline double& eIf(int pi, int ei) { return X[ieIf(pi, ei)]; }
    inline double& psCf(int pi, int si) { return X[ipsCf(pi, si)]; }
    inline double& pCf(int pi) { return X[ipCf(pi)]; }
    inline double& s1sCf(int pi, int si) { return X[is1sCf(pi, si)]; }
    inline double& s1Cf(int pi) { return X[is1Cf(pi)]; }
    inline double& psCp(int pi, int si) { return X[ipsCp(pi, si)]; }
    inline double& pCp(int pi) { return X[ipCp(pi)]; }
    inline double& psCpb(int pi, int si)  { return X[ipsCpb(pi, si)]; }
    inline double& pCpb(int pi)  { return X[ipCpb(pi)]; }

    inline double& pPHf(int pi) { return X[ipPHf(pi)]; }
    inline double& s1PHf(int pi) { return X[is1PHf(pi)]; }
    inline double& pPHp(int pi) { return X[ipPHp(pi)]; }
    inline double& pPHpb(int pi) { return X[ipPHpb(pi)]; }

    inline double& pIf(int pi) { return X[ipIf(pi)]; }
    inline double& s1If(int pi) { return X[is1If(pi)]; }
    inline double& e1Qp(int pi) { return eQp(pi, 0); }
    inline double e1vQp(int pi) { return e1Qp(pi) * eV(pi, 0); }
    inline double e1Qf(int pi) { return e1vQf(pi) / eV(pi, 0); }
    inline double& e1vQc(int pi) { return X[ie1vQc(pi)]; }
    inline double e1Qc(int pi) { return e1vQc(pi) / eV(pi, 0); } // by one vessel
    //inline double& s1Cf(int pi) { retburn eCf(pi, 0); }
    inline double& s1Cp(int pi) { return X[is1Cp(pi)]; }
    inline double& s1Cc(int pi) { return X[is1Cc(pi)]; }
    inline double& s1PIp(int pi) { return X[is1PIp(pi)]; }
    inline double& s1PIc(int pi) { return X[is1PIc(pi)]; }
    inline double& s1PIf(int pi) { return X[is1PIf(pi)]; }
    inline double& s1Pf(int pi) { return X[is1Pf(pi)]; }
    inline double& pIp(int pi) { return X[ipIp(pi)]; }
    inline double& pIpb(int pi) { return X[ipIpb(pi)]; }

    inline double& psCraw(int pi, int si) { return pi == 0 ? syssCaf(si) : psCpb(pi-1, si); } // on the first pass feed = sys feed, on the others feed = previous permeate
    inline double& pCraw(int pi) { return pi == 0 ? sysCaf : pCpb(pi-1); } // on the first pass feed = sys feed, on the others feed = previous permeate
    inline double& pIraw(int pi) { return pi == 0 ? sysIaf : pIpb(pi-1); } // on the first pass feed = sys feed, on the others feed = previous permeate
    inline double& pPHraw(int pi) { return pi == 0 ? sysPHaf : pPHpb(pi-1); } // on the first pass feed = sys feed, on the others feed = previous permeate

    // constant values
    inline int& MSi(int pi, int ei) { return msi[peOff[pi]+ei]; }
    inline double& eS(int pi, int ei) { return s[peOff[pi]+ei]; }
//    inline double evS(int pi, int ei) { return eS(pi, ei) / eV(pi, ei); } // by one vessel
    inline double& eV(int pi, int ei) { return v[peOff[pi]+ei]; }
    inline double& e1V(int pi) { return eV(pi, 0); }
    inline double& e1S(int pi) { return eS(pi, 0); }

    inline double& ePp(int pi, int ei) { return pp[peOff[pi]+ei]; }
//    inline double s1vS(int pi) { return s1S(pi) / eV(pi, 0); } // by one vessel
    

};

#endif // ROSYSTEMSOLVER_H
