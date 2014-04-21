#include "romembrane.h"

ROMembrane::ROMembrane(const QString& series, const QString& model, const ROMembraneSize& membraneSize,
                       double activeArea, double productivity,
                       double rejection, double pressure)
    : _series(series), _model(model), _membraneSize(membraneSize), _activeArea(activeArea),
      _productivity(productivity), _rejection(rejection), _pressure(pressure), QObject() {}

ROMembrane::ROMembrane() : _series("None"), _model("None"), _membraneSize(ROMembraneSize(0.0, 0.0)), _activeArea(0.0),
    _productivity(0.0), _rejection(0.0), _pressure(0.0) {}

ROMembrane::ROMembrane(const ROMembrane& membrane) :
_series(membrane.series()), _model(membrane.model()),
   _membraneSize(membrane.membraneSize()){
   _activeArea = membrane.activeArea();
   _productivity = membrane.productivity();
   _rejection = membrane.rejection();
   _pressure = membrane.pressure();
}


const QString& ROMembrane::series() const { return _series; }
const QString& ROMembrane::model() const { return _model; }

int ROMembrane::seriesIndex() const {
    if (series() == "KM") return 0;
    if (series() == "K") return 1;
    if (series() == "KH") return 2;
    if (series() == "KCH") return 3;
    if (series() == "NF") return 4;
    return -1;
}
const QString ROMembrane::seriesAndModel() const { return QString("%1 %2").arg(_series, _model); }
const ROMembraneSize ROMembrane::membraneSize() const { return _membraneSize; }
double ROMembrane::activeArea() const { return _activeArea; }
double ROMembrane::productivity() const { return _productivity; }
double ROMembrane::rejection() const { return _rejection; }
double ROMembrane::pressure() const { return _pressure; }

double ROMembrane::_aCoeff[] = {
    0.07, //KM
    0.123385, //K
    0.24,// 0.18214, // KH
    0.3, //KCH
    0.25//0.19 // NF
};

SPmCoeffsStruct ROMembrane::_SPmCoeffs[] = {
    // KM
    { -2.29e-09, 2.474e-06, 0.01198 }, //NH4
    { 4.557e-09, -4.946e-06, 0.009049 }, //K
    { 1.122e-09, -1.235e-06, 0.002012 }, //Na
    { 3.322e-09, -3.699e-06, 0.001037 }, //Mg
    { 4.523e-09, -4.942e-06, 0.001049 }, //Ca
    { 4.523e-09, -4.942e-06, 0.001049 }, //Fe
    { 4.512e-09, -4.941e-06, 0.001049 }, //Ba
    { 4.523e-09, -4.942e-06, 0.001049 }, //Sr
    { 1.495e-11, -1.106e-07, 0.002257 }, //Cl
    { 5.679e-09, -6.18e-06, 0.02506 }, //NO3
    { 3.322e-09, -3.699e-06, 0.003037 }, //F
    { 1.122e-11, -1.235e-09, 0.0002 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 2.895e-07, -0.0003274, 0.04225 }, //HCO3
    { 0.0, 0.0, 0.0 }, //CO2
    { 5.657e-06, -0.0006178, 0.08061 }, //Boron
    { 3.367e-08, -3.704e-07, 0.003 }, //SiO2

    // K
    {-4.834e-08, 5.643e-05, 0.02944 },  //NH4
    {-7.829e-08, 9.083e-05, 0.0351 },   //K
    {-7.026e-09, 8.551e-06, 0.005915 }, //Na
    {-9.663e-09, 1.106e-05, 0.00289 },  //Mg
    {-7.363e-09, 8.588e-06, 0.002915 }, //Ca
    {-7.363e-09, 8.588e-06, 0.002915 }, //Fe
    {-7.452e-09, 8.598e-06, 0.002915 }, //Ba
    {-7.363e-09, 8.588e-06, 0.002915 }, //Sr
    {-2.909e-10, 2.264e-06, 0.006149 }, //Cl
    {-1.825e-07, 0.0002123, 0.0959 },   //NO3
    {-1.476e-08, 1.718e-05, 0.00683 },  //F
    {-1.526e-09, 2.39e-06,  0.001976 },  //SO4
    {0.0, 0.0, 0.0 },                    //CO3
    {2.795e-07, -0.0003141, 0.04511 },  //HCO3
    {0.0, 0.0, 0.0 },                     //CO2
    { 1.156e-06, -0.0001238, 0.4501 },   //B
    { -3.805e-06, 0.0004085, 0.0002953 }, //{ -4.59e-06, 0.0004949, -0.0004903 }, //SiO2

    // KH
    { -7.396e-08, 8.591e-05, 0.05815 }, //NH4
    { -1.248e-07, 0.0001448, 0.05956 }, //K
    { -2.231e-08, 2.579e-05, 0.007744 }, //Na
    { -8.171e-09, 9.788e-06, 0.004903 }, // Mg
    { -5.96e-09, 7.322e-06, 0.004927 }, // Ca
    { -5.96e-09, 7.322e-06, 0.004927 }, // Fe
    { -1.608e-08, 1.844e-05, 0.003817 }, // Ba
    { -5.948e-09, 7.321e-06, 0.004927 }, // Sr
    { -4.923e-10, 3.812e-06, 0.01055 }, // Cl
    { -2.813e-07, 0.0003265, 0.1808 }, // NO3
    { -2.652e-08, 3.07e-05, 0.0117 }, // F
    { -5.679e-09, 7.291e-06, 0.002928 }, // SO4
    { 0.0, 0.0, 0.0 }, // CO3
    { 2.635e-07, -0.0002945, 0.04792 }, // HCO3
    { 0.0, 0.0, 0.0 }, // CO2
    { -2.211e-06, 0.0002465, 0.5798 }, // B
    { 4.377e-06, -0.0004926, 0.01049 }, // SiO2

    // KCN
    { -2.096e-07, 0.0002419, 0.2656 }, //NH4
    { -2.826e-08, 3.311e-05, 0.01267 }, //K
    { -3.395e-08, 3.929e-05, 0.01161 }, //Na
    { -1.9e-08, 2.209e-05, 0.006781 }, //Mg
    { -1.566e-08, 1.839e-05, 0.006818 }, //Ca
    { -1.566e-08, 1.839e-05, 0.006818 }, //Fe
    { -1.461e-08, 1.716e-05, 0.00683 }, //Ba
    { -1.689e-08, 1.964e-05, 0.006805 }, //Sr
    { -9.781e-10, 7.549e-06, 0.04194 }, //Cl
    { 0.0, 0.0, 0.0 }, //NO3
    { -8.272e-08, 9.577e-05, 0.08405 }, //F
    { 2.413e-09, -2.654e-07, 0.008002 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 2.795e-07, -0.0003141, 0.04511 }, //HCO3 // -1.991e-07, 0.000233, 0.2687
    { 0.0, 0.0, 0.0 }, //CO2
    { 0.0, 0.0, 0.0 }, //Boron
    { 4.377e-06, -0.0004926, 0.01049 }, //{ 0.0, -0.0007374, 0.02073 }, //SiO2


    // NF
    { -2.29e-09, 2.474e-06, 0.332 }, //NH4
    { 3.356e-09, -3.702e-06, 0.03804 }, //K
    { 1.01e-10, -1.111e-08, 0.036 }, //Na
    { -1.066e-09, 1.228e-06, 0.01499 }, //Mg
    { 4.523e-09, -4.942e-06, 0.01505 }, //Ca
    { 4.523e-09, -4.942e-06, 0.01505 }, //Fe
    { -3.311e-09, 3.698e-06, 0.01396 }, //Ba
    { 4.523e-09, -4.942e-06, 0.01505 }, //Sr
    { -1.124e-11, 7.934e-08, 0.05189 }, //Cl
    { 0.0, 0.0, 0.0 }, //NO3
    { 2.211e-09, -2.465e-06, 0.138 }, //F
    { -3.412e-09, 3.709e-06, 0.007963 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 2.414e-08, -2.599e-05, 0.3893 }, //HCO3
    { 0.0, 0.0, 0.0 }, //CO2
    { 0.0, 0.0, 0.0 }, //B
    { 1.077e-06, -0.000123, 0.03012 } //SiO2
};

SPmCoeffsStruct ROMembrane::SPmCoeffs(int series, int si) {
    int index = series * 17 + si;
    if (0 <= index && index < 5*17)
        return _SPmCoeffs[index];
    else {
        SPmCoeffsStruct none = {0,0,0};
        return none;
    }
}

double ROMembrane::aCoeff(int series) {
    if (0 <= series && series < 5)
        return _aCoeff[series];
    return 0.0;
}
