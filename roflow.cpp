#include "roflow.h"
#include <QObject>
#include <QQueue>

#include "romath.h"

ROFlow::ROFlow() :
    _solutes(new ROSolutes()), _rate(0.0),
     _pressure(0.0), _osmoticPressure(0.0) {
    connect(_solutes, SIGNAL(pHChanged()), this, SIGNAL(pHChanged()));
    connect(_solutes, SIGNAL(solutesChanged()), this, SIGNAL(solutesChanged()));
    connect(_solutes, SIGNAL(temperatureChanged()), this, SIGNAL(temperatureChanged()));

    connect(_solutes, SIGNAL(solutesChanged()), this, SLOT(updateOsmoticPressure()));
    connect(this, SIGNAL(temperatureChanged()), this, SLOT(updateOsmoticPressure()));
}

ROFlow::~ROFlow(){
    delete _solutes;
    _solutes = nullptr;
}

double ROFlow::rate() const { return _rate; }
double ROFlow::pressure() const { return _pressure; }
double ROFlow::osmoticPressure() const {
    return PI(solutes(), temperature());// _osmoticPressure;
}

ROSolutes* const ROFlow::solutes() const { return _solutes; }

void ROFlow::setRate(double rate) { _rate = rate; emit rateChanged(); }//TODO CHECK CONSTS

void ROFlow::setPressure(double value) { _pressure = value; emit pressureChanged(); }
void ROFlow::updateOsmoticPressure() {
    _osmoticPressure = PI(solutes(), temperature());
    emit osmoticPressureChanged();
}

void ROFlow::mix(const ROFlow* other) {
    solutes()->beginChange();
    for (int si = 0; si < ROSolutes::TotalIons; ++si) {
        solutes()->setMeql(si, solutes()->meql(si) + other->solutes()->meql(si));
    }
    solutes()->endChange();
    setRate(this->rate() + other->rate());
    setTemperature((this->temperature() + other->temperature())/2);
    setPH((this->pH() + other->pH())/2);
}

void ROFlow::subtract(const ROFlow* other) {
    solutes()->beginChange();
    for (int si = 0; si < ROSolutes::TotalIons; ++si) {
        solutes()->setMeql(si, solutes()->meql(si) - other->solutes()->meql(si));
    }
    solutes()->endChange();
    setRate(this->rate() - other->rate());
    setTemperature((this->temperature() - other->temperature())/2);
    setPH((this->pH() - other->pH())/2);
}

void ROFlow::copySolutesFrom(const ROFlow* other) {
    solutes()->beginChange();
    for (int si = 0; si < ROSolutes::TotalIons; ++si) {
        solutes()->setMeql(si, other->solutes()->meql(si));
    }
    solutes()->endChange();
    setPH(other->pH());
}

void ROFlow::reset() {
    setRate(0.0);
    setTemperature(25.0);
    setPH(7.6);
    solutes()->reset();
}

void ROFlow::copyDataFrom(const ROFlow* other) {
    this->setRate(other->rate());
    this->setTemperature(other->temperature());
    this->setPH(other->pH());
    this->setPressure(other->pressure());
    this->copySolutesFrom(other);
}

ROFlow* ROFlow::clone() {
    ROFlow* newFlow = new ROFlow();
    newFlow->copyDataFrom(this);
    return newFlow;
}



