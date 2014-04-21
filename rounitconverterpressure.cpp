#include "rounitconverterpressure.h"

ROUnitConverterPressure::ROUnitConverterPressure(Units current, QObject *parent) :
    _currentUnits(current), QObject(parent)
{
}

double ROUnitConverterPressure::convert(double value, Units from, Units to) {
    if (from == Current) from = currentUnits();
    if (to == Current) to = currentUnits();
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
    //}
    return result;
}

double ROUnitConverterPressure::currentToDefault(double value) { return convert(value, Current, Default); }
double ROUnitConverterPressure::defaultToCurrent(double value) { return convert(value, Default, Current); }

ROUnitConverterPressure::Units ROUnitConverterPressure::currentUnits() const {return _currentUnits;}
void ROUnitConverterPressure::setCurrentUnits(Units value) { _currentUnits = value; Q_EMIT currentUnitsChanged(); }
