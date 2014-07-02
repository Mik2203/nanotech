#include "rounitstext.h"
#include <QStringList>

ROUnitsText::ROUnitsText(QObject *parent) :
    QObject(parent) {

    update();
}

ROUnitsText::~ROUnitsText() {
    for (int uIdx=0; uIdx<ROUnits::FLOW_UNIT_COUNT; ++uIdx)
        delete[] _flowUnitsData[uIdx];
    delete[] _flowUnitsData;

    for (int uIdx=0; uIdx<ROUnits::FLUX_UNIT_COUNT; ++uIdx)
        delete[] _fluxUnitsData[uIdx];
    delete[] _fluxUnitsData;

    for (int uIdx=0; uIdx<ROUnits::PRESSURE_UNIT_COUNT; ++uIdx)
        delete[] _pressureUnitsData[uIdx];
    delete[] _pressureUnitsData;

    for (int uIdx=0; uIdx<ROUnits::TEMPERATURE_UNITS_COUNT; ++uIdx)
        delete[] _temperatureUnitsData[uIdx];
    delete[] _temperatureUnitsData;
}

QVariant ROUnitsText::flowUnitsTextList() { return _flowUnitsTextList; }
QVariant ROUnitsText::fluxUnitsTextList() { return _fluxUnitsTextList; }
QVariant ROUnitsText::pressureUnitsTextList() { return _pressureUnitsTextList; }
QVariant ROUnitsText::temperatureUnitsTextList() { return _temperatureUnitsTextList; }

QString ROUnitsText::flowUnitText(int u) {
    if (u < 0 || ROUnits::FLOW_UNIT_COUNT <= u) u = ROUnits::DEFAULT_FLOW_UNITS();
    return tr(_flowUnitsData[u]);
}
QString ROUnitsText::fluxUnitText(int u) {
    if (u < 0 || ROUnits::FLUX_UNIT_COUNT <= u) u = ROUnits::DEFAULT_FLUX_UNITS();
    return tr(_fluxUnitsData[u]);
}
QString ROUnitsText::pressureUnitText(int u) {
    if (u < 0 || ROUnits::PRESSURE_UNIT_COUNT <= u) u = ROUnits::DEFAULT_PRESSURE_UNITS();
    return tr(_pressureUnitsData[u]);
}
QString ROUnitsText::temperatureUnitText(int u) {
    if (u < 0 || ROUnits::TEMPERATURE_UNITS_COUNT <= u) u = ROUnits::DEFAULT_TEMPERATURE_UNITS();
    return tr(_temperatureUnitsData[u]);
}


void ROUnitsText::update() {
    QStringList flowUnitsTextList;
    for (int uIdx=0; uIdx<ROUnits::FLOW_UNIT_COUNT; ++uIdx)
        flowUnitsTextList << flowUnitText(uIdx);
    _flowUnitsTextList = QVariant::fromValue(flowUnitsTextList);

    QStringList fluxUnitsTextList;
    for (int uIdx=0; uIdx<ROUnits::FLUX_UNIT_COUNT; ++uIdx)
        fluxUnitsTextList << fluxUnitText(uIdx);
    _fluxUnitsTextList = QVariant::fromValue(fluxUnitsTextList);

    QStringList pressureUnitsTextList;
    for (int uIdx=0; uIdx<ROUnits::PRESSURE_UNIT_COUNT; ++uIdx)
        pressureUnitsTextList << pressureUnitText(uIdx);
    _pressureUnitsTextList = QVariant::fromValue(pressureUnitsTextList);

    QStringList temperatureUnitsTextList;
    for (int uIdx=0; uIdx<ROUnits::TEMPERATURE_UNITS_COUNT; ++uIdx)
        temperatureUnitsTextList << temperatureUnitText(uIdx);
    _temperatureUnitsTextList = QVariant::fromValue(temperatureUnitsTextList);

    Q_EMIT textUpdated();
}

const char * ROUnitsText::_flowUnitsData[] = {
    QT_TR_NOOP("m³/h"),
    QT_TR_NOOP("m³/d"),
    QT_TR_NOOP("GPD"),
    QT_TR_NOOP("GPM")
};

const char * ROUnitsText::_fluxUnitsData[] = {
    QT_TR_NOOP("lmh"),
    QT_TR_NOOP("GFD")
};

const char * ROUnitsText::_pressureUnitsData[] = {
    QT_TR_NOOP("bar"),
    QT_TR_NOOP("PSI"),
    QT_TR_NOOP("MPa"),
    QT_TR_NOOP("atm")
};

const char * ROUnitsText::_temperatureUnitsData[] = {
    QT_TR_NOOP("C"),
    QT_TR_NOOP("F")
};
