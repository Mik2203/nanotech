#include "roelement.h"

#include "ropass.h"
#include "rosystem.h"
#include "romath.h"
#include "rosolutemodel.h"
#define _USE_MATH_DEFINES // for MSVC 2010 C++
#include <math.h>
#ifdef QT_DEBUG
#include <QDebug>
#endif

ROElement::ROElement(ROStage* stage, ROFlow* feed):
    _stage(stage), _feed(0),
    _permeate(new ROFlow()),
    _concentrate(new ROFlow()), ROAbstractElement() {


    setFeed(feed);

    // TOTAL ACTIVE AREA
    connect(stage, SIGNAL(membraneIndexChanged()), this, SIGNAL(activeAreaChanged()));

    // RECOVERY
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));
    connect(feed, SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));

    // AVERAGE FLUX
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(averageFluxChanged()));
    connect(this, SIGNAL(activeAreaChanged()), this, SIGNAL(averageFluxChanged()));
}
ROElement::ROElement(): _feed(0), _permeate(0),_concentrate(0) {}

ROElement::~ROElement() {
    delete _permeate;
    delete _concentrate;
}

ROFlow* const ROElement::feed() const { return _feed; }
ROFlow* const ROElement::permeate() const { return _permeate; }
ROFlow* const ROElement::concentrate() const { return _concentrate; }
double ROElement::recovery() const { return feed() ? permeate()->rate() / feed()->rate() : 0.0; }
ROStage* const ROElement::stage() const { return _stage; }
void ROElement::setFeed(ROFlow* const newFeed) { _feed = newFeed; emit feedChanged(); }
double ROElement::activeArea() const { return stage()->membrane() ? stage()->membrane()->activeArea() : 0.0; }
ROElement* ROElement::clone(ROFlow *newFeed) { return new ROElement(this->stage(), newFeed); }

