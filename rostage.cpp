#include "rostage.h"
#include "ropass.h"
#include "rosystem.h"
#include "rosolutemodel.h"
#include "romath.h"
#include "rodatabase.h"
#include "romembranesmodel.h"

#define _USE_MATH_DEFINES // for MSVC 2010 C++
#include <math.h>
#ifdef QT_DEBUG
#include <QDebug>
#endif

const int ROStage::MAX_ELEMENTS_PER_VESSEL = 8;
const int ROStage::MIN_ELEMENTS_PER_VESSEL = 1;
const int ROStage::MAX_VESSELS_COUNT = 100;
const int ROStage::MIN_VESSELS_COUNT = 1;

ROStage::ROStage(ROPass* pass, ROFlow* feed,
                 int elementsPerVesselCount, int vesselCount) :
    _pass(pass), _feed(0), _membrane(0),
    _vesselCount(qBound(MIN_VESSELS_COUNT, vesselCount, MAX_VESSELS_COUNT)),
    /*_membrane(0), */
    _permeate(new ROFlow()),
    _firstElementFeed(new ROFlow()),
    _concentrate(new ROFlow()),
    _membraneId(-1),
    _preStagePressure(0.0),
    _backPressure(0.0),
    ROAbstractElement() {

    setFeed(feed);

    int boundElementsPerVesselCount = qBound(MIN_ELEMENTS_PER_VESSEL, elementsPerVesselCount, MAX_ELEMENTS_PER_VESSEL);
    _elements.append(new ROElement(this, _firstElementFeed)); // first element
    for (int vIdx = 1; vIdx < boundElementsPerVesselCount; ++vIdx) {
        _elements.append(new ROElement(this, _elements.at(vIdx-1)->concentrate()));
    }

    //TOTAL ACTIVE AREA
    connect(this, SIGNAL(elementsPerVesselCountChanged()), this, SIGNAL(totalActiveAreaChanged()));
    connect(this, SIGNAL(vesselsCountChanged()), this, SIGNAL(totalActiveAreaChanged()));
    connect(this, SIGNAL(membraneIdChanged()), this, SIGNAL(totalActiveAreaChanged()));

    // RECOVERY
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));
    connect(feed, SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));

    // ELEMENTS COUNT
    connect(this, SIGNAL(elementsPerVesselCountChanged()), this, SIGNAL(elementsCountChanged()));
    connect(this, SIGNAL(vesselsCountChanged()), this, SIGNAL(elementsCountChanged()));

    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(averageFluxChanged()));
    connect(this, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(averageFluxChanged()));
}
ROStage::ROStage(): _feed(0), _permeate(0),_concentrate(0), _firstElementFeed(0), _membrane(0) {}

ROPass* const ROStage::pass() const { return _pass; }

ROStage::~ROStage() {
    delete _permeate;
    delete _concentrate;
    delete _firstElementFeed;
    qDeleteAll(_elements); _elements.clear();
}


ROFlow* const ROStage::feed() const { return _feed; }
ROFlow* const ROStage::permeate() const { return _permeate; }
ROFlow* const ROStage::concentrate() const { return _concentrate; }

int ROStage::vesselCount() const { return _vesselCount; }
int ROStage::elementsPerVesselCount() const { return _elements.count(); }

int ROStage::elementsCount() const {
    return vesselCount() * elementsPerVesselCount();
}
ROMembrane* const ROStage::membrane() const { return _membrane; }

void ROStage::setVesselCount(int vesselCount) {
    _vesselCount = qBound(MIN_VESSELS_COUNT, vesselCount, MAX_VESSELS_COUNT);
    Q_EMIT vesselsCountChanged();
}

int ROStage::membraneId() const { return _membraneId; }

void ROStage::setElementsPerVesselCount(int elementsPerVessel) {

    int boundElementsPerVesselCount = qBound(MIN_ELEMENTS_PER_VESSEL, elementsPerVessel, MAX_ELEMENTS_PER_VESSEL);
    if (boundElementsPerVesselCount != _elements.count()) {
        while (boundElementsPerVesselCount > _elements.count()) {
            _elements.append(new ROElement(this, _elements.last()->concentrate()));
        }
        while (boundElementsPerVesselCount < _elements.count()) {
            ROElement* lastElement = _elements.takeLast();
            delete lastElement;
        }
        Q_EMIT elementsPerVesselCountChanged();
    }
}


void ROStage::setMembraneId(int membraneId) {
    roDB->membranes()->getMembraneData(_membrane, roDB->membranes()->indexById(membraneId));

    _membraneId = membraneId;
    Q_EMIT membraneIdChanged();
}

void ROStage::setFeed(ROFlow* const newFeed) {
    _feed = newFeed;
    Q_EMIT feedChanged();
}


ROStage* ROStage::clone(/*int newStageNumber, */ROFlow* newFeed) {
    ROStage* newStage = new ROStage(this->pass(),/* newStageNumber, */newFeed);
    newStage->copyDataFrom(this);
    return newStage;
}

void ROStage::copyDataFrom(const ROStage* const other) {
    this->setMembraneId(other->membraneId());
    this->setPreStagePressure(other->preStagePressure());
    this->setElementsPerVesselCount(other->elementsPerVesselCount());
    this->setVesselCount(other->vesselCount());
}

double ROStage::activeArea() const {
    double totalArea = 0.0;
    if (membrane()) totalArea = elementsCount() * membrane()->activeArea();
    return totalArea;
}


double ROStage::preStagePressure() const { return _preStagePressure; }
void ROStage::setPreStagePressure(double value) {
    _preStagePressure = qMax(0.0, value); // TODO check
    Q_EMIT preStagePressureChanged();
}

double ROStage::backPressure() const { return _backPressure; }

void ROStage::setBackPressure(double value) {
    _backPressure = qMax(0.0, value); // TODO check
    Q_EMIT backPressureChanged();
}

ROElement* ROStage::element(int elementIndex)  const { return _elements.value(elementIndex); }

int ROStage::elementIndex(ROElement *element) const { return _elements.indexOf(element); }
ROElement* const ROStage::firstElement() const { return _elements.first(); }
ROElement* const ROStage::lastElement() const { return _elements.last();  }


void ROStage::reset() {
    setMembraneId(-1);
    setVesselCount(1);
    setElementsPerVesselCount(1);
}
