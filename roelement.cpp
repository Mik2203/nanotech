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
    _stage(stage), _feed(feed),
    _permeate(new ROFlow()),
    _concentrate(new ROFlow()), ROAbstractElement() {

    bind();
}
ROElement::ROElement(): _feed(0), _permeate(0),_concentrate(0) {}

ROElement::~ROElement() {
    delete _permeate; const_cast<ROFlow*>(_permeate) = nullptr;
    delete _concentrate; const_cast<ROFlow*>(_concentrate) = nullptr;
}

ROFlow* const ROElement::feed() const { return _feed; }
ROFlow* const ROElement::permeate() const { return _permeate; }
ROFlow* const ROElement::concentrate() const { return _concentrate; }
ROStage* const ROElement::stage() const { return _stage; }
void ROElement::setFeed(ROFlow* const newFeed) {
    unbindFeed();
    _feed = newFeed;
    bindFeed();
    Q_EMIT feedChanged();
}
double ROElement::activeArea() const { return stage()->membrane() ? stage()->membrane()->activeArea() : 0.0; }
ROElement* ROElement::clone(ROFlow *newFeed) { return new ROElement(this->stage(), newFeed); }

void ROElement::bind()
{
    bindStage();
    bindPermeate();
    bindFeed();

    connect(this, SIGNAL(activeAreaChanged()), this, SIGNAL(averageFluxChanged()));
}

void ROElement::bindStage()
{
    connect(_stage, SIGNAL(membraneIdChanged()), this, SIGNAL(activeAreaChanged()));
}

void ROElement::bindPermeate()
{
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(averageFluxChanged()));
}

void ROElement::bindFeed()
{
    connect(_feed, SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));
}

void ROElement::unbindFeed()
{
    if (_feed) disconnect(_feed, 0, this, 0);
}

