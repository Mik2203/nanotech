#ifndef ROMATH_H
#define ROMATH_H

#include <QMap>
#include <QDebug>

#define _USE_MATH_DEFINES // for MSVC 2010 C++
#include <math.h>

#include "rosolutes.h"
#include "rosolutemodel.h"
#include "romembrane.h"


/* здесь собраны основные константы, формулы и расчеты.
 */


static const double PSIToBar = 0.06894757;
static const double barToPSI = 14.5037744;
static const double kgfcm2ToPSI = 14.22334;
static const double m3ToL = 1000.028;
static const double kgToL = 1000.0;
static const double lToKg = 1.0 / kgToL;
static const double LToM3 = 1.0 / m3ToL;
static const double gfd_PSItoLmh_bar = 24.62448197;
static const double m3hToGpm = 1.0 / 0.22712472;

static double poly(double x, double* ks, int deg) {
    double res = 0.0;
    for (int d=0; d <= deg; ++d) {
        int ki = deg - d;
        res += pow(x, d) * ks[ki];
    }
    return res;
}


static double dpoly(double x, double* ks, int deg) {
    double res = 0.0;
    for (int d=1; d <= deg; ++d) {
        int ki = deg - d;
        res += d * pow(x, d-1) * ks[ki];
    }
    return res;
}

static double TCF(double T) {
    double TCF = 0;
    if (T >= 25.0)
        TCF = pow(M_E, 2640.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
    else
        TCF = pow(M_E, 3020.0 * (1.0 / 298.0 - 1.0 / (273.0 + T)));
    return TCF;
}

static double Kp(double qp, double qf, double spm) { //SPs
    double y = qp / qf;
    return (1 - pow(1 - y, spm)) / y;
}

//static double KcSPmks[] = { -19.7688586, 30.61790274, -16.5186939, 1.872681555, -0.11863331 };
//static double Kc(double qp, double qf, double spm){ //{ #pow(1-Yi, SPm-1)
//    double y = qp / qf;
////    return pow(1 - Y, SPm - 1);
//    return (spm - 1) * poly(y, KcSPmks, 4) + 1.0;
//}

static double pfi(double qp, double qf) {
    double y = qp / qf;
    return exp(0.7 * y);
}

static double dpfiQf(double qp, double qf) {
    double y = qp / qf;
    return -0.7 * y * pfi(qp, qf) / (qf);
}

static double dpfiQp(double qp, double qf) {
    return 0.7 * pfi(qp, qf) / qf;
}

static double PIi(double Cf, double Qp, double Qf, double Cc, /*double T, */double PIf) {
    return pfi(Qp, Qf) * PIf * 0.5 * (1 + Cc / Cf) * barToPSI;
}

static double dPIiQf(double Cf, double Qp, double Qf, double Cc, /*double T, */double PIf) {
    return dpfiQf(Qp, Qf) * PIf * 0.5 * (1 + Cc / Cf) * barToPSI; //* pfi = math.exp(0.7 * Qp / Qf)
}

static double dPIiQp(double Cf, double Qp, double Qf, double Cc, /*double T, */double PIf){
    return dpfiQp(Qp, Qf) * PIf * 0.5 * (1 + Cc / Cf) * barToPSI; //* pfi = math.exp(0.7 * Qp / Qf)
}

static double dPIiCc(double Cf, double Qp, double Qf, /*double T, */double PIf) {
    return pfi(Qp, Qf) * PIf * 0.5 * barToPSI / Cf;
}

static double dPIiCf(double Cf, double Qp, double Qf, double Cc, /*double T,*/ double PIf) {
    return - pfi(Qp, Qf) * PIf * 0.5 * Cc * barToPSI / (Cf * Cf);
}

static double A(int series, double Cf, double Qp, double Qf, double Cc, double PIf) {
    double a = 0;
    double pii = PIi(Cf, Qp, Qf, Cc, PIf);
    double aCoeff = ROMembrane::aCoeff(series);
    if (pii <= 25.0)
        a = aCoeff;
    else if (pii <= 200)
        a = aCoeff - 0.011 * (pii - 25.0) / 35.0; // TODO optimize
    else
        a = aCoeff - 0.0001 * (pii - 200);
    return a * gfd_PSItoLmh_bar;
}

static double dAQf(
        double Cf,
        double Qp,
        double Qf,
        double Cc,
        double PIf) {
    double da = 0;
    double pii = PIi(Cf, Qp, Qf, Cc, PIf);
    if (pii <= 25.0) da = 0.0;
    else if (pii <= 200) da = -0.011 / 35.0 * dPIiQf(Cf, Qp, Qf, Cc, PIf); // TODO optimize
    else da = -0.0001 * dPIiQf(Cf, Qp, Qf, Cc, PIf);
    return da * gfd_PSItoLmh_bar;
}

static double dAQp(double Cf, double Qp, double Qf, double Cc, double PIf) {
    double da = 0;
    double pii = PIi(Cf, Qp, Qf, Cc, PIf);
    if (pii <= 25.0) da = 0.0;
    else if (pii <= 200) da = -0.011 / 35.0 * dPIiQp(Cf, Qp, Qf, Cc, PIf); // TODO optimize
    else da = -0.0001 * dPIiQp(Cf, Qp, Qf, Cc, PIf);
    return da * gfd_PSItoLmh_bar;
}

static double dACf(double Cf, double Qp, double Qf, double Cc, double PIf) {
    double da = 0;
    double pii = PIi(Cf, Qp, Qf, Cc, PIf);
    if (pii <= 25.0) da = 0.0;
    else if (pii <= 200) da = -0.011 / 35.0 * dPIiCf(Cf, Qp, Qf, Cc, PIf); // TODO optimize
    else da = -0.0001 * dPIiCf(Cf, Qp, Qf, Cc, PIf);
    return da * gfd_PSItoLmh_bar;
}

static double dACc(double Cf, double Qp, double Qf, double Cc, double PIf) {
    double da = 0;
    double pii = PIi(Cf, Qp, Qf, Cc, PIf);
    if (pii <= 25.0) da = 0.0;
    else if (pii <= 200) da = -0.011 / 35.0 * dPIiCc(Cf, Qp, Qf, PIf); // TODO optimize
    else da = -0.0001 * dPIiCc(Cf, Qp, Qf, PIf);
    return da * gfd_PSItoLmh_bar;
}


static double dPfc(double qf, double qc) {
    double qfqc = qf + qc;
    return pow(qfqc, 1.7) * 0.0026370507231639933;
}

static double ddPfcQf(double qf, double qc) {
    double qfqc = qf + qc;
    return pow(qfqc, 0.7) * 1.7 * 0.0026370507231639933;
}
static double ddPfcQc(double qf, double qc) {
    double qfqc = qf + qc;
    return pow(qfqc, 0.7) * 1.7 * 0.0026370507231639933;
}

static double Pf(
        double Qf,
        double Qp,
        double Qc,
        double Cf,
        double Cc,
        double PIf,
        double PIp,
        double PIc,
        double S,
        double T,
        double FF,
        int series) {
    double a = A(series, Cf, Qp, Qf, Cc, PIf);
    double tcf = TCF(T);
    if (a == 0.0 || S == 0.0 || tcf == 0.0 || FF == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD Pf";
#endif
        a = 1.0;
        S = 1.0;
        tcf = 1.0;
        FF = 1.0;
    }
    return 0.5 * (PIf + PIc) - PIp + \
           0.5 * dPfc(Qf, Qc) + \
           (Qp * m3ToL) / (a * S * tcf * FF);
}

static double dPfQf(
        double Qf,
        double Qp_,
        double Qc,
        double Cf,
        double Cc,
        double PIf,
        double S,
        double T,
        double FF,
        int series) {
    double a = A(series, Cf, Qp_, Qf, Cc, PIf);
    double tcf = TCF(T);
    if (a == 0.0 || S == 0.0 || tcf == 0.0 || FF == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dPfQf";
#endif
        a = 1.0;
        S = 1.0;
        tcf = 1.0;
        FF = 1.0;
    }
    return 0.5 * ddPfcQf(Qf, Qc) - \
           Qp_ * m3ToL * dAQf(Cf, Qp_, Qf, Cc, PIf) / (S * tcf * FF * a * a);
}


static double dPfQp(
        double Qf,
        double Qp_,
        double Cf,
        double Cc,
        double PIf,
        double S,
        double T,
        double FF,
        int series) {
    double a = A(series, Cf, Qp_, Qf, Cc, PIf);
    double tcf = TCF(T);
    if (a == 0.0 || S == 0.0 || tcf == 0.0 || FF == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dPfQp";
#endif
        a = 1.0;
        S = 1.0;
        tcf = 1.0;
        FF = 1.0;
    }
    return m3ToL / (S * tcf * FF) * (a - Qp_ * dAQp(Cf, Qp_, Qf, Cc, PIf)) / (a * a);
}

static double dPfQc(
        double Qf,
        double Qc) {
    return 0.5 * ddPfcQc(Qf, Qc);
}

static double dPfCf(
        double Qf,
        double Qp_,
        double Cf,
        double Cc,
        double PIf,
        double S,
        double T,
        double FF,
        int series) {
    double a = A(series, Cf, Qp_, Qf, Cc, PIf);
    double tcf = TCF(T);
    if (a == 0.0 || S == 0.0 || tcf == 0.0 || FF == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dPfCf";
#endif
        a = 1.0;
        S = 1.0;
        tcf = 1.0;
        FF = 1.0;
    }
    return Qp_ * m3ToL / (S * tcf * FF) * (-dACf(Cf, Qp_, Qf, Cc, PIf) / (a * a));
}


static double dPfCc(
        double Qf,
        double Qp_,
        double Cf,
        double Cc,
        double PIf,
        double S,
        double T,
        double FF,
        int series) {
    double a = A(series, Cf, Qp_, Qf, Cc, PIf);
    double tcf = TCF(T);
    if (a == 0.0 || S == 0.0 || tcf == 0.0 || FF == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dPfCc";
#endif
        a = 1.0;
        S = 1.0;
        tcf = 1.0;
        FF = 1.0;
    }
    return Qp_ * m3ToL / (S * tcf * FF) * (-dACc(Cf, Qp_, Qf, Cc, PIf) / (a * a));
}


static double dPfPIf() {
    return 0.5;
}

static double dPfPIc() {
    return 0.5;
}

static double dPfPIp() {
    return -1.0;
}

static double dKpQp(double qp, double qf, double spm) //{ return (Kc(qp + h, qf, spm) - Kc(qp - h, qf, spm)) / (2 * h); }
{
    double y = qp / qf;
    return (spm * pow(1 - y, spm - 1) / qp) + (qf * (pow(1 - y, spm) - 1) / (qp * qp));
}

static double dKpQf(double qp, double qf, double spm) //{ return (Kp(qp, qf + h, spm) - Kp(qp, qf - h, spm)) / (2 * h); }
{
    double y = qp / qf;
    return (pow(1 - y, spm) - 1) / qp - spm * pow(1 - y, spm - 1) / qf;
}

static double dKpSPm(double qp, double qf, double spm){ //{ #pow(1-Yi, spm-1)
    double y = qp / qf;
    return -pow(1 - y, spm) * log(1 - y) / y;
}

static double PI(const QMap<int, double>& solutes, double t) {
    double num10 = 0.0;
    double num9 = 0.0;
    double x = 0.0;
    Q_FOREACH(const int &si, solutes.keys()) {
        int ic = ROSolutes::soluteInfos[si].ionicCharge;
        if (ic != 0) {
            num10 += solutes[si] * ROSolutes::molarMass(si);
            num9 += fabs(solutes[si] / ic);
            x += fabs(solutes[si] * ic);
        }
    }

    double num5 = 1e+6 / (1e+6 -  num10);
    num9 = num9 / 1000 * num5;
    x = x / 2000 * num5;

    bool b = (num10 < 1e+6);

    double num6 = 1.0;
    if (b) {
        double num8 = 3.0 / pow(x, 1.5) * (1.0 + sqrt(x) - 1.0 / (1.0 + sqrt(x)) - 2.0 * log(1.0 + sqrt(x)));
        double num11 = 0.0;
        Q_FOREACH(const int &siPos, solutes.keys()) {
            if (ROSolutes::soluteInfos[siPos].ionicCharge < 0) continue;
            Q_FOREACH(const int &siNeg, solutes.keys()) {
                if (ROSolutes::soluteInfos[siNeg].ionicCharge > 0) continue;
                num11 += solutes[siPos] * solutes[siNeg];
            }
        }
        num11 = num11 / 1e+6 * num5 * num5;
        num6 = 1 - (0.72160666651 * (pow(x, 1.5) * num8 - (0.054623 + 0.001676 * t) * num11)) / num9;
    }
//#ifdef QT_DEBUG
//        qDebug() << "is nan ?" << (1.0 + num10 * 7.2E-07) / num5 * 1.206 * num6 * (t + 273.15) * num9 * PSIToBar;
//#endif
    return (1.0 + num10 * 7.2E-07) / num5 * 1.206 * num6 * (t + 273.15) * num9 * PSIToBar;
}

static double dPIsC(const QMap<int, double>& solutes, double t, int si) {
    double num10 = 0.0;
    double dnum10 = 0.0;
    double num9 = 0.0;
    double dnum9 = 0.0;
    double x = 0.0;
    double dx = 0.0;

    // x, etc..
    Q_FOREACH(const int &si, solutes.keys()) {
        int ic = ROSolutes::soluteInfos[si].ionicCharge;
        if (ic != 0) {
            num10 += solutes[si] * ROSolutes::molarMass(si);
            num9 += fabs(solutes[si] / ic);
            x += fabs(solutes[si] * ic);
        }
    }

    // dx, etc..
    int ic = ROSolutes::soluteInfos[si].ionicCharge;
    if (ic != 0) {
        dnum10 = ROSolutes::molarMass(si);
        dnum9 = fabs(1.0 / ic);
        dx = abs(ic);
    }

    double num5 = 1e+6 / (1e+6 - num10);
    double dnum5 = 1e+6 * dnum10 / ((1e+6 - num10) * (1e+6 - num10));
    num9 = num9 / 1000 * num5;
    dnum9 = (dnum9 * num5 + num9 * dnum5) / 1000;
    x = x / 2000 * num5;
    dx = (dx * num5 + x * dnum5) / 2000;

    bool b = (num10 < 1e+6);

    double num6 = 1.0;
    double dnum6 = 0.0;
    if (b) {
        double px = pow(x, 1.5);
        double dpx = 1.5 * pow(x, 0.5) * dx;
        double sqrtx_1 = sqrt(x) + 1.0;
        double dsqrtx_1 = 0.0;

        double num8 = 3.0 / px * (sqrtx_1 - 1.0 / sqrtx_1 - 2.0 * log(sqrtx_1));
        double dnum8 = 3.0 * (px * (dsqrtx_1 / (sqrtx_1 * sqrtx_1) -
                                    2.0 * dsqrtx_1 / sqrtx_1 +
                                    dsqrtx_1) -
                              dpx * (-1.0 / sqrtx_1 -
                                     2.0 * log(sqrtx_1) +
                                     sqrtx_1)) / (px * px);
        double num11 = 0.0;
        double dnum11 = 0.0;


        // calc num11
        Q_FOREACH(const int &siPos, solutes.keys()) {
            if (ROSolutes::soluteInfos[siPos].ionicCharge < 0) continue;
            Q_FOREACH(const int &siNeg, solutes.keys()) {
                if (ROSolutes::soluteInfos[siNeg].ionicCharge > 0) continue;
                num11 += solutes[siPos] * solutes[siNeg];
            }
        }


        // calc dnum11
        if (ic < 0) {
            Q_FOREACH(const int &siPos, solutes.keys()) {
                if (ROSolutes::soluteInfos[siPos].ionicCharge < 0) continue;
                dnum11 += solutes[siPos];
            }
        } else {
            Q_FOREACH(const int &siNeg, solutes.keys()) {
                if (ROSolutes::soluteInfos[siNeg].ionicCharge > 0) continue;
                dnum11 += solutes[siNeg];
            }
        }

        num11 = num11 / 1e+6 * num5 * num5;
        dnum11 = num5 * (num5 * dnum11 + 2 * num11 * dnum5) / 1e+6;
        double tt = (0.054623 + 0.001676 * t);

        num6 = 1 - (0.72160666651 * (px * num8 - tt * num11)) / num9;
        dnum6 = 0.72160666651 * (dnum9 * (num8 * px - tt * num11) -
                                 num9 * (px * dnum8 + 1.5 * num8 * pow(x, 0.5) * dpx - tt * dnum11)) /
                (num9 * num9);
    }
//#ifdef QT_DEBUG
//        qDebug() << "is nan ?" << 1.206 * (t + 273.15) * PSIToBar / (num5 * num5) * (num5 * ((1.0 + 7.2E-07 * num10) *
//                                                                                             (num9 * dnum6 + dnum9 * num6) +
//                                                                                             7.2E-07 * num6 * num9 * dnum10) -
//                                                                                     num6 * num9 * dnum5 * (1.0 + 7.2E-07 * num10));
//#endif
    // return (1.0 + num10 * 7.2E-07) / num5 * 1.206 * num6 * (t + 273.15) * num9 * PSIToBar;
    return 1.206 * (t + 273.15) * PSIToBar / (num5 * num5) * (num5 * ((1.0 + 7.2E-07 * num10) *
                                                                      (num9 * dnum6 + dnum9 * num6) +
                                                                      7.2E-07 * num6 * num9 * dnum10) -
                                                              num6 * num9 * dnum5 * (1.0 + 7.2E-07 * num10));
}

static double PI_old(double tds, double t) {
    return 1.123 / kgfcm2ToPSI * (273.0 + t) * (tds * LToM3);
}

static double dPI_oldTds(double t) {
    return 1.123 / kgfcm2ToPSI * (273.0 + t) * LToM3;
}

static double PI(ROSolutes* solutes, double t) {
    QMap<int, double> solutesMap;
    Q_FOREACH(const int& si, solutes->filledIons()) {
        solutesMap[si] = solutes->meql(si);
    }
    return PI(solutesMap, t);
}

// polarize constants
static double k1(double t) {
    return (-0.0007205786691317941 * t * t + 0.08407290283790285 * t + 2.649160197210195) * 1e-7;
}

static double dk1t(double t) {
    return (-0.0007205786691317941 * t + 0.08407290283790285) * 1e-7;
}

static double k2(double t) {
    return (9.2 * t + 230.0) * 1e-13;
}

static double co2(double hco3, double t, double ph, double i) {
    if (i < 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD co2";
#endif
        return 0.0;
    }
    return hco3 / (k1(t) * pow(10.0, ph + sqrt(i)));
}

static double co3(double hco3, double t, double ph) {
    return k2(t) * hco3 * pow(10.0, ph + 2.0) / ROSolutes::molarMass(ROSolutes::CO3);
}

static double dco3hco3(double t, double ph) {
    return k2(t) * pow(10.0, ph + 2.0) / ROSolutes::molarMass(ROSolutes::CO3);
}

static double dco3ph(double hco3, double t, double ph) {
    return k2(t) * hco3 / ROSolutes::molarMass(ROSolutes::CO3) * log(10.0) * pow(10.0, ph + 2.0);
}

static double ph(double hco3, double co2, double t, double i) {
    if (i < 0.0 || co2 ==0.0 || k1(t) == 0.0 || hco3 / (co2 * k1(t)) <= 0) {
#ifdef QT_DEBUG
        qDebug() << "BAD ph";
#endif
        return 0;
    }
    return log10(hco3 / (co2 * k1(t))) - sqrt(i);
}

static double dphhco3(double hco3) {
    if (hco3 == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD hco3";
#endif
        return 0.0;
    }
    return 1.0 / (hco3 * log(10.0));
}

static double dphco2(double co2) {
    if (co2 == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dphco2";
#endif
        co2 = 1.0;
    }
    return - 1.0 / (co2 * log(10.0));
}

static double dpht(double t) {
    if (k1(t) == 0.0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dpht";
#endif
        return 0.0;
    }
    return -dk1t(t) / (log(10.0) * k1(t));
}

static double dphi(double i) {
    if (i <= 0) {
#ifdef QT_DEBUG
        qDebug() << "BAD dphi";
#endif
        return 0.0;
    }
    return -1.0 / (2.0 * sqrt(i));
}

static double SPm(int series, int si, double c) {
    SPmCoeffsStruct spmc = ROMembrane::SPmCoeffs(series, si);
    return poly(c, reinterpret_cast<double *>(&spmc), 2);
}

static double dSPm(int series, int si, double c) {
    SPmCoeffsStruct spmc = ROMembrane::SPmCoeffs(series, si);
    return dpoly(c, reinterpret_cast<double *>(&spmc), 2);
}

static double SPmPh(int series, double ph) {
    SPmCoeffsStruct spmph = ROMembrane::PhSPmCoeffs(series);
    double * sa = reinterpret_cast<double *>(&spmph);
//    qDebug() << "SA VALS:" << sa[0] << sa[1] << sa[2];
    return qBound(ROMembrane::PhSPmMin(series),
                  poly(ph, reinterpret_cast<double *>(&spmph), 2),
                  ROMembrane::PhSPmMax(series));
}

static double dSPmPh(int series, double ph) {
    double spmph = poly(ph, reinterpret_cast<double *>(&spmph), 2);
    if (spmph < ROMembrane::PhSPmMin(series) || ROMembrane::PhSPmMax(series) < spmph)
        return 0.0;  // no d
    SPmCoeffsStruct spmphcs = ROMembrane::PhSPmCoeffs(series);
    return dpoly(ph, reinterpret_cast<double *>(&spmphcs), 2);
}

#endif // ROMATH_H
