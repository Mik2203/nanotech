#include "rosolutes.h"
#include "romath.h"

#include <QDebug>

ROSolutes::ROSolutes(QObject *parent) :
    QObject(parent),
    _pH(7.6),
    _temperature(25.0),
    _totalMolarMass(0.0),
    _changing(false) {
    _solutesMeql.resize(TotalIons);

    connect(this, SIGNAL(temperatureChanged()), this, SLOT(recalculate()));
    connect(this, SIGNAL(pHChanged()), this, SLOT(recalculate()));
}


const ROSolutes::SoluteInfo ROSolutes::soluteInfos[] = {
    ROSolutes::SoluteInfo(ROSolutes::Cation, 18.0385, 1),         //NH4
    ROSolutes::SoluteInfo(ROSolutes::Cation, 39.0983, 1),
    ROSolutes::SoluteInfo(ROSolutes::Cation, 22.989768, 1),
    ROSolutes::SoluteInfo(ROSolutes::Cation, 24.3050 / 2, 2),
    ROSolutes::SoluteInfo(ROSolutes::Cation, 40.0784 / 2, 2),
    ROSolutes::SoluteInfo(ROSolutes::Cation, 55.8452 / 2, 2),
    ROSolutes::SoluteInfo(ROSolutes::Cation, 137.327 / 2, 2),
    ROSolutes::SoluteInfo(ROSolutes::Cation, 87.621 / 2, 2),


    ROSolutes::SoluteInfo(ROSolutes::Anion, 35.4527, -1), // Cl
    ROSolutes::SoluteInfo(ROSolutes::Anion, 62.00494, -1),
    ROSolutes::SoluteInfo(ROSolutes::Anion, 18.998403, -1),
    ROSolutes::SoluteInfo(ROSolutes::Anion, 96.0631 / 2, -2), // SO4
    ROSolutes::SoluteInfo(ROSolutes::Anion, 60.00907 / 2, -2),
    ROSolutes::SoluteInfo(ROSolutes::Anion, 61.0168, -1), // HCO3

    ROSolutes::SoluteInfo(ROSolutes::Other, 44.0095, 0), // CO2
    ROSolutes::SoluteInfo(ROSolutes::Other, 10.811, 1),
    ROSolutes::SoluteInfo(ROSolutes::Other, 60.0848, 0)
};


double ROSolutes::pH() const { return _pH; }

double ROSolutes::temperature() const { return _temperature; }


double ROSolutes::ionicStrength() const {
    double ic = 0.0;
    for (int si=0; si<TotalIons; ++si){
        if (si != CO2)
            ic += _solutesMeql[si] * abs(soluteInfos[si].ionicCharge);
    }
    return ic * lToKg / 2;
}

double ROSolutes::ionicStrength2() const {
    return 2.5 * 1e-5 * totalValueMgl();
}


bool ROSolutes::isSaturated(int compound) const {
    return saturation(static_cast<ScalingCompound>(compound)) > 0.0;
}

double ROSolutes::saturation(ROSolutes::ScalingCompound compound) const {
    double Ksp = 0.0;
    double IP = 0.0;
    double is = ionicStrength();
    switch (compound) {
    case CaSO4: {
        Ksp = -0.0004 * is * is + 0.0022 * is + 0.00008;
        IP = value(Ca, MolL) * value(SO4, MolL);
        break;
    }
    case BaSO4: {
        Ksp = (-2E-09) * is * is + (1E-08) * is - (3E-11);
        IP = value(Ba, MolL) * value(SO4, MolL);
        break;
    }
    case SrSO4: {
        Ksp = -9E-06 * is * is + 3E-05 * is + 1E-07;
        IP = value(Sr, MolL) * value(SO4, MolL);
        break;
    }
    case CaF2: {
        Ksp = 5.2E-11;
        IP = value(Ca, MolL) * value(F, MolL) * value(F, MolL);
        break;
    }
    case SiO2_: {
        Ksp = 75 + temperature()*2;
        if (_pH < 7.0) Ksp = (-0.1232 * _pH + 1.834) * Ksp;
        else if (_pH > 7.8) Ksp = 0.0087 * exp(0.6043 * _pH) * Ksp;
        IP = value(SiO2, Mgl);

        break;
    }
    case MgOH2: {
        double pOH = 14.0-pH();
        double OH = pow(10.0, -pOH);
        Ksp = 1.2E-11;
        IP = value(Mg, MolL) * OH * OH; //* value(SO4, MeqKgByIc)
        break;
    }
    }

    return IP / Ksp;
}

// TODO CHECK INDEXES
double ROSolutes::mgl(int soluteIndex) const { return _solutesMeql[soluteIndex] * soluteInfos[soluteIndex].molarMass; }
double ROSolutes::meql(int soluteIndex) const { return _solutesMeql[soluteIndex]; }
double ROSolutes::value(int soluteIndex, ROSolutes::Units units) const {
    switch (units) {
    case Mgl: return this->mgl(soluteIndex);
    case Meql: return this->meql(soluteIndex);
    case MeqKg: return this->meql(soluteIndex) * lToKg;
    case MolL: return this->meql(soluteIndex) * lToKg / fabs(ionicCharge(soluteIndex));
    }
    return 0;
}

bool ROSolutes::isIonFilled(int soluteIndex) const { return _solutesMeql.value(soluteIndex) > 0.0; }

bool ROSolutes::isEditable(int soluteIndex) { return (soluteIndex != CO2 && soluteIndex != CO3); }

QVector<int> ROSolutes::filledIons() const {
    QVector<int> filled;
    for (int si = 0; si < ROSolutes::TotalIons; ++si){
        if (isIonFilled(si)) filled << si;
    }
    return filled;
}

QVector<int> ROSolutes::saturatedCompounds() const {
    QVector<int> saturated;
    for (int si = 0; si < ROSolutes::TotalCompounds; ++si){
        if (isSaturated(si)) saturated << si;
    }
    return saturated;
}

double ROSolutes::totalMolarMass(Types type) {
    double totalMolarMass = 0.0;
    for (int si = 0; si < ROSolutes::TotalIons; ++si){
        if (soluteInfos[si].type & type && si != CO2) totalMolarMass += soluteInfos[si].molarMass;
    }
    return totalMolarMass;
}

double ROSolutes::totalValue(ROSolutes::Units units, Types type) const {
    double totalValue = 0.0;
    for (int si = 0; si < _solutesMeql.count(); ++si){
        if (soluteInfos[si].type & type && si != CO2) totalValue += value(si, units);
    }
    return totalValue;
}

double ROSolutes::totalValueMgl() const { return totalValue(Mgl); }

double ROSolutes::molarMass(int si) { return soluteInfos[si].molarMass; }

double ROSolutes::ionicCharge(int si) { return soluteInfos[si].ionicCharge; }

bool ROSolutes::isBalanced() const {
    return qFuzzyCompare(1.0 + totalValue(Meql, Cation), 1.0 + totalValue(Meql, Anion));
}

bool ROSolutes::isFilled() const { return totalValue(Meql) > 0.0; }

void ROSolutes::setPH(double value) {
    _pH = value;
    Q_EMIT pHChanged();
}

void ROSolutes::setTemperature(double value) { _temperature = value; emit temperatureChanged(); }

void ROSolutes::setMgl(int soluteIndex, double mgl) { setMeql(soluteIndex, mgl / soluteInfos[soluteIndex].molarMass); }
void ROSolutes::setMeql(int soluteIndex, double meql) {
    if (0 <= soluteIndex && soluteIndex < TotalIons && isEditable(soluteIndex)) {
        bool localChanging = _changing;
        if (!localChanging) beginChange();
        _solutesMeql[soluteIndex] = meql;
        if (!localChanging) endChange();
        //if (!_changing) Q_EMIT solutesChanged();
    }
}
void ROSolutes::setValue(int soluteIndex, double value, Units units) {
    switch (units) {
    case Mgl: setMgl(soluteIndex, value); break;
    case Meql: setMeql(soluteIndex, value); break;
    case MeqKg: setMeql(soluteIndex, value * kgToL); break;
    case MolL: setMeql(soluteIndex, value * kgToL * fabs(ionicCharge(soluteIndex))); break;
    }
}


void ROSolutes::setTotalValue(double value, Units units, Types type) {
    // TODO PEREDELAT' VSE!!! 4to konkretno dibil ?
    bool _localChanging = _changing;
    if (!_localChanging) beginChange();
    if (value < 0.0) value = 0.0;
    if (type == Anion) {
        double valueCL = this->value(Cl, units);
        double totalDelta = value - totalValue(units, Anion);
        double totalResidual = totalDelta+valueCL;
        if (totalResidual > 0) {
            setValue(Cl, totalResidual, units);
        } else {
            setValue(Cl, 0.0, units);
            double totalValue = 0.0;
            for (int si = 0; si < _solutesMeql.count(); ++si){
                if ((soluteInfos[si].type & Anion) && isEditable(si) && isIonFilled(si)) {
                    totalValue += this->value(si, units);
                }
            }
            double koeff = value / fabs(totalValue);
            for (int si = 0; si < _solutesMeql.count(); ++si){
                if ((soluteInfos[si].type & Anion) && isEditable(si) && isIonFilled(si)) {
                    this->setValue(si, this->value(si, units) * koeff, units);
                }
            }
        }
    }
    else if (type == Cation) {
        double valueCL = this->value(Na, units);
        double totalDelta = value - totalValue(units, Cation);
        double totalResidual = totalDelta+valueCL;
        if (totalResidual > 0) {
            setValue(Na, totalResidual, units);
        } else {
            setValue(Na, 0.0, units);
            double totalValue = 0.0;
            for (int si = 0; si < ROSolutes::TotalIons; ++si){
                if ((soluteInfos[si].type & Cation) && isEditable(si) && isIonFilled(si)) {
                    totalValue += this->value(si, units);
                }
            }
            double koeff = value / fabs(totalValue);
            for (int si = 0; si < ROSolutes::TotalIons; ++si){
                if ((soluteInfos[si].type & Cation) && isEditable(si) && isIonFilled(si)) {
                    this->setValue(si, this->value(si, units) * koeff, units);
                }
            }
        }
    } else {
        double totalValue = 0.0;
        for (int si = 0; si < ROSolutes::TotalIons; ++si){
            if (isEditable(si) && isIonFilled(si)) {
                totalValue += this->value(si, units);
            }
        }
        double koeff = value / fabs(totalValue);
        for (int si = 0; si < ROSolutes::TotalIons; ++si){
            if (isEditable(si) && isIonFilled(si)) {
                this->setValue(si, this->value(si, units) * koeff, units);
            }
        }

    }
    if (!_localChanging) endChange();
}



void ROSolutes::reset() { //Types type
    bool localChanging = _changing;
    if (!localChanging) beginChange();
    for (int si = 0; si < _solutesMeql.count(); ++si){
        if (isEditable(si)) setMeql(si, 0.0);
    }
    if (!localChanging) endChange();
    setTemperature(25.0);
    setPH(7.6);
}

void ROSolutes::recalculate() {
    if (!_changing){
        beginChange();
        endChange();
    }
}

void ROSolutes::beginChange() { _changing = true; }
void ROSolutes::endChange() {
    if (_changing) {
        calcAutoValues();
        _changing = false; Q_EMIT solutesChanged();
    }
}

void ROSolutes::adjust() {
    double totalCations = totalValue(Meql, Cation);
    double totalAnions = totalValue(Meql, Anion);
    if (totalCations > totalAnions) setTotalValue(totalCations, Meql, Anion);
    else if (totalAnions > totalCations) setTotalValue(totalAnions, Meql, Cation);
}

double ROSolutes::lsi() const {
    double i = ionicStrength();
    double ca = meql(ROSolutes::Ca) > 0 ? meql(ROSolutes::Ca) : 1.0;
    double hco3 = meql(ROSolutes::HCO3) > 0 ? meql(ROSolutes::HCO3) : 1.0;
    double pHs = -log10((9.2 * temperature() + 230.0) * ca * hco3 / (9.237 * exp(-0.0277 * temperature()))) + 2 * sqrt(i) / (sqrt(i) + 1) + 10;
    return pH() - pHs;
}

// здесь в последнем коэффициенте также заложен коэффициент при температуре
static double sdsiIks[] = { 0.25760533, -1.5511718, 3.83604767, -5.08686859, 3.69128285, 2.720913894357 };
double ROSolutes::sdsi() const {
    double i = ionicStrength();
    double ca = meql(ROSolutes::Ca) > 0 ? meql(ROSolutes::Ca) : 1.0;
    double hco3 = meql(ROSolutes::HCO3) > 0 ? meql(ROSolutes::HCO3) : 1.0;
    double k = poly(i, sdsiIks, 5) - 0.02085714 * temperature();
    double pHs = -log10(ca * hco3) + k + 6.0;
    return pH() - pHs;
}

double ROSolutes::valueFromTotal(double total, int si) {
    return total * molarMass(si) / totalMolarMass();
}

void ROSolutes::calcAutoValues() {
    _solutesMeql[CO2] = co2(meql(HCO3), temperature(), pH(), ionicStrength());
    _solutesMeql[CO3] = co3(meql(HCO3), temperature(), pH());
}
