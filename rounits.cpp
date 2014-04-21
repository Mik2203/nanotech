#include "rounits.h"
#include "romath.h"

ROUnits::ROUnits(FlowUnits flowU, FluxUnits fluxU, PressureUnits pressU, TemperatureUnits tempU, QObject *parent) :
    QObject(parent), _flowUnits(flowU), _fluxUnits(fluxU), _pressureUnits(pressU), _temperatureUnits(tempU) { }


ROUnits::FlowUnits ROUnits::flowUnits() const { return _flowUnits; }
ROUnits::FluxUnits ROUnits::fluxUnits() const { return _fluxUnits; }
ROUnits::PressureUnits ROUnits::pressureUnits() const { return _pressureUnits; }
ROUnits::TemperatureUnits ROUnits::temperatureUnits() const { return _temperatureUnits; }

ROUnits::FlowUnits ROUnits::DEFAULT_FLOW_UNITS() { return m3h; }
ROUnits::FluxUnits ROUnits::DEFAULT_FLUX_UNITS() { return lmh; }
ROUnits::PressureUnits ROUnits::DEFAULT_PRESSURE_UNITS() { return bar; }
ROUnits::TemperatureUnits ROUnits::DEFAULT_TEMPERATURE_UNITS() { return Celsius; }

void ROUnits::setFlowUnits(FlowUnits u) {
    if (u != _flowUnits) {
        _flowUnits = u;
        Q_EMIT flowUnitsChanged();
    }
}
void ROUnits::setFluxUnits(FluxUnits u) {
    if (u != _fluxUnits) {
        _fluxUnits = u;
        Q_EMIT fluxUnitsChanged();
    }
}
void ROUnits::setPressureUnits(PressureUnits u) {
    if (u != _pressureUnits) {
        _pressureUnits = u;
        Q_EMIT pressureUnitsChanged();
    }
}
void ROUnits::setTemperatureUnits(TemperatureUnits u) {
    if (u != _temperatureUnits) {
        _temperatureUnits = u;
        Q_EMIT temperatureUnitsChanged();
    }
}


double ROUnits::convertFlowUnits(double value, ROUnits::FlowUnits from, ROUnits::FlowUnits to) {
    if (from == to) return value;

    double m3hValue, result;

    switch(from) {
    // TODO CONSTANTS TO ROMATH!
    case m3h: m3hValue = value; break;
    case m3d: m3hValue = value / 24; break;
    case GPD: m3hValue = value * 0.0001577255; break;
    case GPM: m3hValue = value * 0.227124707; break;
    }

    switch(to) {
    case m3h: result = m3hValue; break;
    case m3d: result = m3hValue * 24; break;
    case GPD: result = m3hValue / 0.0001577255; break;
    case GPM: result = m3hValue / 0.227124707; break;
    }
    return result;
}

double ROUnits::convertFluxUnits(double value, ROUnits::FluxUnits from, ROUnits::FluxUnits to) {
    if (from == to) return value;

    double lmhValue, result;

    switch(from) {
    case lmh: lmhValue = value; break;
    case GFD: lmhValue = value * 24.62448197; break;
    }


    switch(to) {
    case lmh: result = lmhValue; break;
    case GFD: result = lmhValue / 24.62448197; break;
    }
    return result;
}

double ROUnits::convertPressureUnits(double value, ROUnits::PressureUnits from, ROUnits::PressureUnits to) {
    if (from == to) return value;

    double barValue, result;

    switch(from) {
    case bar: barValue = value; break;
    case PSI: barValue = value * 0.0689475729; break;
    case MPa: barValue = value * 10; break;
    case atm: barValue = value * 1.01325; break;
    }


    switch(to) {
    case bar: result = barValue; break;
    case PSI: result = barValue / 0.0689475729; break;
    case MPa: result = barValue / 10; break;
    case atm: result = barValue / 1.01325; break;
    }
    return result;
}

double ROUnits::convertTemperatureUnits(double value, ROUnits::TemperatureUnits from, ROUnits::TemperatureUnits to) {
    if (from == to) return value;

    switch(from) {
    case Celsius: {
        switch(to) {
        case Fahrenheit: return value * 1.8 + 32.0;
        }
    }
    case Fahrenheit: {
        switch(to) {
        case Celsius: return (value - 32.0) / 1.8;
        }
    }
    }
    return 0.0;
}
