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
    if (series() == "KM-C") return 0;
    if (series() == "K") return 1;
    if (series() == "KH") return 2;
    if (series() == "KCH") return 3;
    if (series() == "NF") return 4;
    if (series() == "KC") return 5;
    if (series() == "KM-S") return 6;
    return -1;
}
const QString ROMembrane::seriesAndModel() const { return QString("%1 %2").arg(_series, _model); }
const ROMembraneSize ROMembrane::membraneSize() const { return _membraneSize; }
double ROMembrane::activeArea() const { return _activeArea; }
double ROMembrane::productivity() const { return _productivity; }
double ROMembrane::rejection() const { return _rejection; }
double ROMembrane::pressure() const { return _pressure; }

double ROMembrane::_aCoeff[] = {
    0.079, // KM-C
    0.123,  // K
    0.202,  // KH
    0.295,  // KCH
    0.208,  // NF
    0.129,  // KC
    0.0625   // KM-S
};

SPmCoeffsStruct ROMembrane::_SPmCoeffs[] = {
    // KM-C
    { -2.29e-09, 2.474e-06, 0.07 }, //NH4
    { 4.557e-09, -4.946e-06, 0.003594 }, //K
    { 1.122e-09, -1.235e-06, 0.002639 }, //Na
    { 3.322e-09, -3.699e-06, 0.000797 }, //Mg
    { 4.523e-09, -4.942e-06, 0.00107 }, //Ca
    { 4.523e-09, -4.942e-06, 0.00082 }, //Fe
    { 4.512e-09, -4.941e-06, 0.001389 }, //Ba
    { 4.523e-09, -4.942e-06, 0.001101 }, //Sr
    { 1.495e-11, -1.106e-07, 0.00341 }, //Cl
    { 5.679e-09, -6.18e-06, 0.023 }, //NO3
    { 3.322e-09, -3.699e-06, 0.003037 }, //F
    { 1.122e-11, -1.235e-09, 0.001807 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 0.0, 0.0, 0.003054 }, //HCO3
    { 0.0, 0.0, 0.999 }, //CO2
    { 0.0, 0.0, 0.111 }, //Boron
    { 3.367e-08, -3.704e-07, 0.003 }, //SiO2

    // K
    {-4.834e-08, 5.643e-05, 0.0394 },  //NH4
    {-7.829e-08, 9.083e-05, 0.008614 },   //K
    {-7.026e-09, 8.551e-06, 0.007591 }, //Na
    {-9.663e-09, 1.106e-05, 0.004653 },  //Mg
    {-7.363e-09, 8.588e-06, 0.005247 }, //Ca
    {-7.363e-09, 8.588e-06, 0.004707 }, //Fe
    {-7.452e-09, 8.598e-06, 0.005839 }, //Ba
    {-7.363e-09, 8.588e-06, 0.005502 }, //Sr
    {-2.909e-10, 2.264e-06, 0.008431 }, //Cl
    {-1.825e-07, 0.0002123, 0.0499 },   //NO3
    {-1.476e-08, 1.718e-05, 0.007013 },  //F
    {-1.526e-09, 2.39e-06,  0.006501 },  //SO4
    { 0.0, 0.0, 0.0 },                    //CO3
    { 0.0, 0.0, 0.008058 },  //HCO3
    { 0.0, 0.0, 0.995 },                     //CO2
    { 0.0, 0.0, 0.468 },   //B
    { -3.805e-06, 0.0004085, 0.004 }, //SiO2

    // KH
    { -7.396e-08, 8.591e-05, 0.05815 }, //NH4
    { -1.248e-07, 0.0001448, 0.008412 }, //K
    { -2.231e-08, 2.579e-05, 0.00772 }, //Na
    { -8.171e-09, 9.788e-06, 0.005533 }, // Mg
    { -5.96e-09, 7.322e-06, 0.006005 }, // Ca
    { -5.96e-09, 7.322e-06, 0.005577 }, // Fe
    { -1.608e-08, 1.844e-05, 0.006458 }, // Ba
    { -5.948e-09, 7.321e-06, 0.006202 }, // Sr
    { -4.923e-10, 3.812e-06, 0.00829 }, // Cl
    { -2.813e-07, 0.0003265, 0.1808 }, // NO3
    { -2.652e-08, 3.07e-05, 0.007315 }, // F
    { -5.679e-09, 7.291e-06, 0.006947 }, // SO4
    { 0.0, 0.0, 0.0 }, // CO3
    { 0.0, 0.0, 0.00804 }, // HCO3
    { 0.0, 0.0, 0.995 }, // CO2
    { 0.0, 0.0, 0.5 }, // B
    { 4.377e-06, -0.0004926, 0.0111 }, // SiO2

    // KCH
    { -2.096e-07, 0.0002419, 0.2656 }, //NH4
    { -2.826e-08, 3.311e-05, 0.01698 }, //K
    { -3.395e-08, 3.929e-05, 0.013737 }, //Na
    { -1.9e-08, 2.209e-05, 0.006042 }, //Mg
    { -1.566e-08, 1.839e-05, 0.007393 }, //Ca
    { -1.566e-08, 1.839e-05, 0.006161 }, //Fe
    { -1.461e-08, 1.716e-05, 0.008845 }, //Ba
    { -1.689e-08, 1.964e-05, 0.008006 }, //Sr
    { -9.781e-10, 7.549e-06, 0.016379 }, //Cl
    { 0.0, 0.0, 0.367 }, //NO3
    { -8.272e-08, 9.577e-05, 0.012027 }, //F
    { 2.413e-09, -2.654e-07, 0.010592 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 0.0, 0.0, 0.015184 }, //HCO3 // -1.991e-07, 0.000233, 0.2687
    { 0.0, 0.0, 0.999 }, //CO2
    { 0.0, 0.0, 0.6 }, //Boron
    { 4.377e-06, -0.0004926, 0.0124 }, //{ 0.0, -0.0007374, 0.02073 }, //SiO2


    // NF
    { -2.29e-09, 2.474e-06, 0.6 }, //NH4
    { 3.356e-09, -3.702e-06, 0.525779 }, //K
    { 1.01e-10, -1.111e-08, 0.329474 }, //Na
    { -1.066e-09, 1.228e-06, 0.053853 }, //Mg
    { 4.523e-09, -4.942e-06, 0.084023 }, //Ca
    { 4.523e-09, -4.942e-06, 0.056206 }, //Fe
    { -3.311e-09, 3.698e-06, 0.124774 }, //Ba
    { 4.523e-09, -4.942e-06, 0.100157 }, //Sr
    { -1.124e-11, 7.934e-08, 0.485622 }, //Cl
    { 0.0, 0.0, 0.635041 }, //NO3
    { 2.211e-09, -2.465e-06, 0.245765 }, //F
    { -3.412e-09, 3.709e-06, 0.18569 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 0.0, 0.0, 0.410872 }, //HCO3
    { 0.0, 0.0, 1.0 }, //CO2
    { 0.0, 0.0, 0.7 }, //B
    { 1.077e-06, -0.000123, 0.034 }, //SiO2

    // KC
    {-4.834e-08, 5.643e-05, 0.0286 },  //NH4
    {-7.829e-08, 9.083e-05, 0.006868 },   //K
    {-7.026e-09, 8.551e-06, 0.003992 }, //Na
    {-9.663e-09, 1.106e-05, 0.000487 },  //Mg
    {-7.363e-09, 8.588e-06, 0.000817 }, //Ca
    {-7.363e-09, 8.588e-06, 0.000512 }, //Fe
    {-7.452e-09, 8.598e-06, 0.001293 }, //Ba
    {-7.363e-09, 8.588e-06, 0.001002 }, //Sr
    {-2.909e-10, 2.264e-06, 0.006263 }, //Cl
    {-1.825e-07, 0.0002123, 0.04 },   //NO3
    {-1.476e-08, 1.718e-05, 0.00284 },  //F
    {-1.526e-09, 2.39e-06,  0.002051 },  //SO4
    { 0.0, 0.0, 0.0 },                    //CO3
    { 0.0, 0.0, 0.005158 },  //HCO3
    { 0.0, 0.0, 0.995 },                     //CO2
    { 0.0, 0.0, 0.467 },   //B
    { -3.805e-06, 0.0004085, 0.0034 }, //SiO2

    // KM-S
    { -2.29e-09, 2.474e-06, 0.0753 }, //NH4
    { 4.557e-09, -4.946e-06, 0.00257 }, //K
    { 1.122e-09, -1.235e-06, 0.00197 }, //Na
    { 3.322e-09, -3.699e-06, 0.000703 }, //Mg
    { 4.523e-09, -4.942e-06, 0.000906 }, //Ca
    { 4.523e-09, -4.942e-06, 0.000721 }, //Fe
    { 4.512e-09, -4.941e-06, 0.001134 }, //Ba
    { 4.523e-09, -4.942e-06, 0.001001 }, //Sr
    { 1.495e-11, -1.106e-07, 0.002457 }, //Cl
    { 5.679e-09, -6.18e-06, 0.0208 }, //NO3
    { 3.322e-09, -3.699e-06, 0.003037 }, //F
    { 1.122e-11, -1.235e-09, 0.001422 }, //SO4
    { 0.0, 0.0, 0.0 }, //CO3
    { 0.0, 0.0, 0.002234 }, //HCO3
    { 0.0, 0.0, 0.999 }, //CO2
    { 0.0, 0.0, 0.1105 }, //Boron
    { 3.367e-08, -3.704e-07, 0.003 } //SiO2
};

SPmCoeffsStruct ROMembrane::SPmCoeffs(int series, int si) {
    int index = series * 17 + si;
    if (0 <= index && index < 7*17)
        return _SPmCoeffs[index];
    SPmCoeffsStruct none = {0,0,0};
    return none;
}

double ROMembrane::aCoeff(int series) {
    if (0 <= series && series < 7)
        return _aCoeff[series];
    return 0.0;
}
