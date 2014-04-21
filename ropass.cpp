#include "ropass.h"
#include "rosystem.h"
#include "romath.h"


#ifdef QT_DEBUG
#include <QDebug>
#endif

//#include "rostage.h"

ROPass::ROPass(ROSystem* system, /*int passNumber,*/ ROFlow* feed) :
    _system(system),
    _hasSelfRecycle(false),
    _blendedRecycledFeed(new ROFlow()),
    _totalStagesPermeate(new ROFlow()),
    _firstStageFeed(new ROFlow()),
    _flowChanging(false), _permeateChanging(false),
    _feed(0), _permeate(new ROFlow()),
    _concentrate(new ROFlow()),
    _selfRecycle(0.0/*new ROFlow()*/),
    _flowFactor(1.0),
    //_recycleUpdated(false),
    _recovery(0.0),
    _hasBlendPermeate(false), _blendPermeate(0.0),
    ROAbstractElement() {

    ROStage* firstStage = new ROStage(this,/* 0, */_firstStageFeed);
    _stages << firstStage;
    setFeed(feed);

    // TOTAL ACTIVE AREA
    connect(firstStage, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(totalActiveAreaChanged()));
    connect(this, SIGNAL(stageCountChanged()), this, SIGNAL(totalActiveAreaChanged()));

    // ELEMENTS COUNT
    connect(firstStage, SIGNAL(elementsCountChanged()), this, SIGNAL(elementsCountChanged()));

    // AVERAgE FLUX
    connect(this, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(averageFluxChanged()));
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(averageFluxChanged()));


    // POWER & ENERGY -
    // достаточно только реагировать на изменения давления, потому что даже, если что то еще поменяется,
    // то нужен пересчет, который изменит давление
    connect(firstStageFeed(), SIGNAL(pressureChanged()), this, SIGNAL(powerChanged()));
    connect(this, SIGNAL(powerChanged()), this, SIGNAL(specificEnergyChanged()));


    connect(permeate(), SIGNAL(rateChanged()), this, SLOT(updateRecycles()));
    connect(permeate(), SIGNAL(rateChanged()), this, SLOT(updateBlendPermeate()));

    emit totalActiveAreaChanged();
}


const int ROPass::_MAX_STAGES_COUNT = 4;
const int ROPass::_MIN_STAGES_COUNT = 1;

const double ROPass::_MAX_RECOVERY = 0.99;
const double ROPass::_MIN_RECOVERY = 0.01;



ROPass* ROPass::clone(ROFlow* const newFeed) {
    ROPass* newPass = new ROPass(this->system(), newFeed);
    newPass->copyDataFrom(this);
    return newPass;
}


void ROPass::copyDataFrom(const ROPass* const from) {
    this->setFlowFactor(from->flowFactor());
    this->feed()->setRate(from->feed()->rate());
    this->permeate()->setRate(from->permeate()->rate());
    this->setRecovery(from->recovery());
    this->firstStage()->copyDataFrom(from->firstStage());
    this->setStageCount(from->stageCount());

    for (int sIdx = 0; sIdx < from->stageCount(); ++sIdx){
        this->stage(sIdx)->copyDataFrom(from->stage(sIdx));
    }

    if (from->hasSelfRecycle()) this->setSelfRecycle(from->selfRecycle());
    // other recycles are copied by system, because system is owner
}

void ROPass::setFeed(ROFlow* newFeed) {
    if (_feed) {
        disconnect(_feed, 0, this, 0);
    }
    Q_EMIT beginFeedChange();
    _feed = newFeed;
    if (_feed) {
        connect(_feed, SIGNAL(rateChanged()), this, SLOT(updateRecycles()));
    } else {
        // TODO LOG
    }

    Q_EMIT feedChanged();
}

ROPass::ROPass(): _feed(0), _system(0), _concentrate(0), _permeate(0), _firstStageFeed(0), _blendedRecycledFeed(0), _totalStagesPermeate(0), _selfRecycle(0)/*, _CW_FillFlowData(0)*/ {}
ROPass::~ROPass() {
    delete _blendedRecycledFeed;
    delete _firstStageFeed;
    delete _concentrate;
    delete _permeate;
    delete _totalStagesPermeate;
    qDeleteAll(_stages); _stages.clear();
}

ROSystem* ROPass::system() const { return _system; }
ROFlow* const ROPass::permeate() const { return _permeate; }
ROFlow* const ROPass::totalStagesPermeate() const { return _totalStagesPermeate; }
ROFlow* const ROPass::concentrate() const { return _concentrate; }
ROFlow* const ROPass::feed() const { return _feed; }
ROFlow* const ROPass::blendedRecycledFeed() const { return _blendedRecycledFeed; }
ROFlow* const ROPass::firstStageFeed() const { return _firstStageFeed; }
double ROPass::recovery() const { return _recovery; }
int ROPass::stageCount() const { return _stages.count(); }

int ROPass::elementsCount() const {
    int elsCount = 0;
    Q_FOREACH(const ROStage* stage, _stages) {
        elsCount += stage->elementsCount();
    }
    return elsCount;
}


void ROPass::setRecovery(double value) {
    _recovery =  value; //qBound(_MIN_RECOVERY, value, _MAX_RECOVERY);
    Q_EMIT recoveryChanged();
}

ROStage* ROPass::addStage(int copyFromStageNumber) {
    if (_stages.count() < _MAX_STAGES_COUNT) {
        ROFlow* feed = _stages.last()->concentrate();
        ROStage* newStage;
        if (0 <= copyFromStageNumber && copyFromStageNumber < _stages.count())
            newStage = stage(copyFromStageNumber)->clone(/*_stages.count(), */feed);
        else
            newStage = new ROStage(this, /*_stages.count(),*/ feed);

        _stages.append(newStage);
        connect(newStage, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(totalActiveAreaChanged()));
        connect(newStage, SIGNAL(elementsCountChanged()), this, SIGNAL(elementsCountChanged()));

        Q_EMIT stageCountChanged();
        Q_EMIT lastStageChanged();
        return newStage;
    }
    return 0;
}



bool ROPass::removeStage(int stageIndex) {
    if (stageIndex == -1) stageIndex = _stages.count()-1; // last
    if (_stages.count() > _MIN_STAGES_COUNT && (0 <= stageIndex && stageIndex < _stages.count())) {
        ROStage *removingStage = _stages.takeAt(stageIndex);
        if (stageIndex == 0) {
            _stages[stageIndex]->setFeed(_firstStageFeed);
            Q_EMIT firstStageChanged();
        } else if (stageIndex < _stages.count()){
            _stages[stageIndex]->setFeed(_stages[stageIndex-1]->concentrate());
        } else { // last stage has been removed
            Q_EMIT lastStageChanged();
        }
        delete removingStage;
        Q_EMIT stageCountChanged();
        return true;
    }
    return false;
}

int ROPass::stageIndex(ROStage* stage) const { return _stages.indexOf(stage); }

bool ROPass::setStageCount(int stageCount) {
    if (_MIN_STAGES_COUNT <= stageCount && stageCount <= _MAX_STAGES_COUNT) {
        while (stageCount > _stages.count()) addStage();
        while (stageCount < _stages.count()) removeStage();
        return true;
    }
    return false;
}

void ROPass::addRecycle(int toPassIdx, double rate) {
    return addRecycle(system()->pass(toPassIdx), rate);
}

void ROPass::addRecycle(const ROPass *const toPass, double rate) {
    if (toPass == this) {
        setSelfRecycle(rate);
    } else {
        // check value to be less than (concentrate rate - exists recycles)
        double totalRecycles = 0.0;
        auto passOutRecycles = outgoingRecycles();
        auto ro = passOutRecycles.constBegin();
        int toPassIndex = system()->passIndex(toPass);
        double recycleValue = -1.0; // means does not exists
        while (ro != passOutRecycles.constEnd()) {
            int rpo = ro.key(); // recycle pass index
            if (toPassIndex != rpo) totalRecycles += ro.value();
            else recycleValue = ro.value();
            ++ro;
        }
        double rateResidual = qMax(0.0, feed()->rate() - permeate()->rate() - totalRecycles);
        double newRecycleRate = qBound(0.0, rate, rateResidual);
        if (recycleValue != newRecycleRate) {
            system()->setPassRecycle(this, toPass, newRecycleRate);
            Q_EMIT recycleChanged();
        }
    }
}

void ROPass::removeRecycle(int toPassIdx) {
    removeRecycle(system()->pass(toPassIdx));
}

void ROPass::removeRecycle(const ROPass *const toPass) {
    if (toPass == this) {
        removeSelfRecycle();
    }
    else {
        system()->removePassRecycle(this, toPass);
        emit recycleChanged();
    }
}


bool ROPass::hasRecycle(int toPassIdx) const {
    return hasRecycle(system()->pass(toPassIdx));
}

bool ROPass::hasRecycle(const ROPass *const toPass) const {
    if (toPass == this) return hasSelfRecycle();
    else return system()->hasRecycle(this, toPass);
}

/*ROFlow**/ double ROPass::recycle(int recyclePassNumber) const {
    if (recyclePassNumber == system()->passIndex(this)) return selfRecycle();
    else return system()->recycle(system()->passIndex(this), recyclePassNumber);
}

ROStage* ROPass::stage(int stageNumber) const { return _stages.value(stageNumber); }

double ROPass::activeArea() const {
    double totalArea = 0.0;
    Q_FOREACH(const ROStage* stage, _stages) {
        totalArea += stage->activeArea();
    }
    return totalArea;
}

bool ROPass::hasSelfRecycle() const { return _hasSelfRecycle; }
double ROPass::selfRecycle() const { return _hasSelfRecycle ? _selfRecycle : 0.0; }

void ROPass::removeSelfRecycle() {
    if (_hasSelfRecycle) {
        _hasSelfRecycle = false;
        Q_EMIT hasSelfRecycleChanged();
    }
    _selfRecycle = 0.0;
    Q_EMIT selfRecycleChanged();
}

bool ROPass::hasBlendPermeate() const { return _hasBlendPermeate; }

double ROPass::flowFactor() const { return _flowFactor; }

double ROPass::blendPermeate() const { return _hasBlendPermeate ? _blendPermeate : 0.0; }

double ROPass::power() const {
    return firstStageFeed()->pressure() * firstStageFeed()->rate() * 0.03472222222;
}

double ROPass::specificEnergy() const {
    return permeate()->rate() > 0.0 ? power() / permeate()->rate() : 0.0;
}

void ROPass::setFlowFactor(double value) {
    // TODO bounds to properties
    _flowFactor = qBound(0.01, value, 1.0); // TODO CONSTS
    Q_EMIT flowFactorChanged();
}

void ROPass::setBlendPermeate(double value) {
    if (!_hasBlendPermeate) {
        _hasBlendPermeate = true;
        Q_EMIT hasBlendPermeateChanged();
    }
    double newValue = qBound(0.0, value, permeate()->rate());
    if (_blendPermeate != newValue) {
        _blendPermeate = newValue;
        Q_EMIT blendPermeateChanged();
    }
}

void ROPass::setSelfRecycle(double value) {
    if (!_hasSelfRecycle) {
        _hasSelfRecycle = true;
        Q_EMIT hasSelfRecycleChanged();
    }
    if (_selfRecycle != value) {
        _selfRecycle = qMax(0.0, value);
        Q_EMIT selfRecycleChanged();
    }
}

void ROPass::removeBlendPermeate() {
    if (_hasBlendPermeate) {
        _hasBlendPermeate = false;
        Q_EMIT hasBlendPermeateChanged();
    }
}

const QMap<int, double> ROPass::outgoingRecycles() const {
    return system()->passOutgoingRecycles(system()->passIndex(this));
}

const QMap<int, double> ROPass::incomingRecycles() const {
    return system()->passIncomingRecycles(system()->passIndex(this));
}

ROStage* const ROPass::firstStage() const { return _stages.first(); }
ROStage* const ROPass::lastStage() const { return _stages.last(); }


void ROPass::updateBlendPermeate() {
    if (_hasBlendPermeate) setBlendPermeate(qBound(0.0, _blendPermeate, permeate()->rate()));
}

void ROPass::updateRecycles() {
    double totalRecycles = 0.0;
    Q_FOREACH(const double& r, outgoingRecycles()) totalRecycles += r;
    double rateResidual = feed()->rate() - permeate()->rate();

    if (totalRecycles > rateResidual) {
        // if recycles rates more than concentrate rate
        // then they should be trimmed or removed
        auto passOutRecycles = outgoingRecycles();
        auto ro = passOutRecycles.constBegin();
        while (ro != passOutRecycles.constEnd()) {
            int rpo = ro.key(); // recycle pass index
            if (ro.value() < rateResidual) rateResidual -= ro.value();
            else if (rateResidual > 0.0) {
                addRecycle(rpo, rateResidual);
                rateResidual = 0.0;
            } else {
                removeRecycle(rpo);
            }
            ++ro;
        }
        Q_EMIT recycleChanged();
    }
}

void ROPass::reset() {
    setFlowFactor(1.0);
    removeBlendPermeate();
    removeSelfRecycle();
    setStageCount(1);
    firstStage()->reset();
    feed()->setRate(0.0);
    setRecovery(0.0);
    permeate()->setRate(0.0);
}
