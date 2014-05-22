#include "rosystem.h"
#include "romath.h"

#include "rodatabase.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif


const int ROSystem::_MAX_PASSES_COUNT = 2;
const int ROSystem::_MIN_PASSES_COUNT = 1;

const int ROSystem::_MAX_FEEDS_COUNT = 4;
const int ROSystem::_MIN_FEEDS_COUNT = 1;

ROSystem::ROSystem() :
    _feedsToResultFeed_STH(new ROFlowMixer(ROFlowMixer::FlowTemperature | ROFlowMixer::FlowSolutes/* | ROFlowMixer::FlowPH*/)),
    _adjustedFeedToResultFeed_R(new ROFlowMixer(ROFlowMixer::FlowRate)),
    _concentrate(new ROFlow()),
    _resultFeed(new ROFlow()),
    _waterTypeIndex(-1),
    _lifetime(1),
    ROAbstractElement(){

    ROFeed* feed = new ROFeed();
    _feeds.append(feed);
    _feedsToResultFeed_STH->setOutputFlow(_resultFeed);
    _feedsToResultFeed_STH->addFeed(feed, ROFlowMixer::FlowAdd);


    _scalingElement = new ROScalingElement(_resultFeed);

    _adjustedFeedToResultFeed_R->setOutputFlow(_resultFeed);
    _adjustedFeedToResultFeed_R->addFeed(adjustedFeed(), ROFlowMixer::FlowAdd);

    ROPass* pass = new ROPass(this, adjustedFeed());
    _passes << pass;

    // ENERGY
    connect(pass, SIGNAL(powerChanged()), this, SIGNAL(powerChanged()));
    connect(pass, SIGNAL(specificEnergyChanged()), this, SIGNAL(specificEnergyChanged()));

    // RECOVERY
    connect(_resultFeed, SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged()));
    connect(this, SIGNAL(passCountChanged()), this, SLOT(refreshPermeate())); refreshPermeate();
    connect(this, SIGNAL(permeateChanged()), this, SIGNAL(recoveryChanged()));

    // RAW WATER FLOW
    connect(_resultFeed, SIGNAL(temperatureChanged()), this, SIGNAL(temperatureChanged()));
    connect(_resultFeed, SIGNAL(pHChanged()), this, SIGNAL(pHChanged()));

    // ELEMENTS COUNT
    connect(this, SIGNAL(passCountChanged()), this, SIGNAL(elementsCountChanged()));
    connect(pass, SIGNAL(elementsCountChanged()), this, SIGNAL(elementsCountChanged()));

    // STAGES COUNT
    connect(this, SIGNAL(passCountChanged()), this, SIGNAL(stagesCountChanged()));
    connect(pass, SIGNAL(stageCountChanged()), this, SIGNAL(stagesCountChanged()));

    // TOTAL AREA
    connect(pass, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(totalActiveAreaChanged()));
    connect(this, SIGNAL(passCountChanged()), this, SIGNAL(totalActiveAreaChanged()));

    // AVERAGE FLUX
    connect(this, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(averageFluxChanged()));
    connect(permeate(), SIGNAL(rateChanged()), this, SIGNAL(averageFluxChanged()));

    // first pass flow factor
    connect(this, SIGNAL(waterTypeIndexChanged()), pass, SIGNAL(flowFactorChanged()));

    Q_EMIT totalActiveAreaChanged();
}

ROSystem::~ROSystem() {
    qDeleteAll(_passes); _passes.clear();
    qDeleteAll(_feeds); _feeds.clear();
    delete _concentrate;
    delete _resultFeed;
    delete _feedsToResultFeed_STH;
    delete _adjustedFeedToResultFeed_R;
    delete _scalingElement;
}

ROFlow* const ROSystem::feed() const { return _resultFeed; }
ROFlow *const ROSystem::adjustedFeed() const { return _scalingElement->adjustedFeed(); }

ROFeed* ROSystem::partFeed(int feedIndex) const { return _feeds.value(feedIndex, 0); }
int ROSystem::passCount() const { return _passes.count(); }

ROFlow* const ROSystem::concentrate() const { return _concentrate; }

ROFeed* ROSystem::addPartFeed(int copyFromFeedIndex) {
    if (_feeds.count() < _MAX_FEEDS_COUNT) {
        ROFeed* newFeed = new ROFeed(1.0, 0, this);;
        if (0 <= copyFromFeedIndex && copyFromFeedIndex < _feeds.count()) {
            newFeed->copyDataFrom(partFeed(copyFromFeedIndex));
        }
        _feeds.append(newFeed);


        _feedsToResultFeed_STH->addFeed(newFeed, ROFlowMixer::FlowAdd);
        emit feedCountChanged();
        return newFeed;
    }
    return 0;
}


/*ROFlow**/ double ROSystem::recycle(int fromPassIdx, int toPassIdx) const {
    return _passRecycles.contains(fromPassIdx) ? _passRecycles.value(fromPassIdx).value(toPassIdx, 0.0) : 0.0;
}

double ROSystem::recycle(const ROPass *const fromPass, const ROPass *const toPass) const {
    return recycle(passIndex(fromPass), passIndex(toPass));
}

void ROSystem::setPassRecycle(int fromPassIdx, int toPassIdx, double value) {
    if (0 <= fromPassIdx && fromPassIdx < passCount() && toPassIdx < fromPassIdx) {
        _passRecycles[fromPassIdx][toPassIdx] = value;
    }
}

void ROSystem::setPassRecycle(const ROPass *const fromPass, const ROPass *const toPass, double value) {
    setPassRecycle(passIndex(fromPass), passIndex(toPass), value);
}

ROPass* ROSystem::pass(int passIndex) const { return _passes.value(passIndex); }
ROPass* ROSystem::firstPass() const { return _passes.first(); }
ROPass* ROSystem::lastPass() const { return _passes.last(); }

ROFeed* ROSystem::firstPartFeed() const { return _feeds.first(); }
ROFeed* ROSystem::lastPartFeed() const { return _feeds.last(); }

int ROSystem::passIndex(ROPass* pass) const { return _passes.indexOf(pass); }

int ROSystem::passIndex(const ROPass *const pass) const { return passIndex(const_cast<ROPass* const>(pass)); }
int ROSystem::partFeedIndex(ROFeed* feed) const { return _feeds.indexOf(feed); }

ROPass* ROSystem::addPass(int copyFromPassNumber) {
    if (_passes.count() < _MAX_PASSES_COUNT) {
        ROFlow* feed = _passes.last()->permeate();
        ROPass* newPass;
        if (0 <= copyFromPassNumber && copyFromPassNumber < _passes.count()) {
            newPass = pass(copyFromPassNumber)->clone(/*_passes.count(), */feed);
        }
        else
            newPass = new ROPass(this,/* _passes.count(), */feed);
        _passes.append(newPass);
        Q_EMIT lastPassChanged();

        // TOTAL AREA
        connect(newPass, SIGNAL(totalActiveAreaChanged()), this, SIGNAL(totalActiveAreaChanged()));
        connect(newPass, SIGNAL(powerChanged()), this, SIGNAL(powerChanged()));
        connect(newPass, SIGNAL(specificEnergyChanged()), this, SIGNAL(specificEnergyChanged()));

        // ELEMENTS COUNT
        connect(newPass, SIGNAL(elementsCountChanged()), this, SIGNAL(elementsCountChanged()));
        connect(newPass, SIGNAL(stageCountChanged()), this, SIGNAL(stagesCountChanged()));


        emit passCountChanged();
        return newPass;
    }
    return 0;
}

bool ROSystem::removePass(int passIndex) {
    if (passIndex == -1) passIndex = _passes.count()-1; // last
    if (_passes.count() > _MIN_PASSES_COUNT &&
            (0 <= passIndex && passIndex < _passes.count())) {

        ROPass* removingPass = _passes.takeAt(passIndex);

        if (passIndex == 0) {  // first pass
            ROPass* newFirstPass = _passes[passIndex];
            newFirstPass->setFeed(adjustedFeed());

            // first pass flow factor
            connect(this, SIGNAL(waterTypeIndexChanged()), newFirstPass, SIGNAL(flowFactorChanged()));
            Q_EMIT waterTypeIndexChanged(); // TODO how to do it like this: newFirstPass->flowFactorChanged() ?

            // first pass SP increase
            connect(this, SIGNAL(lifetimeChanged()), newFirstPass, SIGNAL(saltPassageYearIncreaseChanged()));
            Q_EMIT lifetimeChanged(); // TODO how to do it like this: newFirstPass->flowFactorChanged() ?

            Q_EMIT firstPassChanged();
        } else if (passIndex < _passes.count()){
            _passes[passIndex]->setFeed(_passes[passIndex-1]->permeate());
        } else { // last pass has been removed
            Q_EMIT lastPassChanged();
        }
        removePassRecycle(passIndex);

        delete removingPass;

        emit passCountChanged();
        return true;
    }
    return false;
}

bool ROSystem::setPassCount(int passCount) {
    if (_MIN_PASSES_COUNT <= passCount && passCount <= _MAX_PASSES_COUNT) {
        while (passCount > _passes.count()) addPass();
        while (passCount < _passes.count()) removePass();
        return true;
    }
    return false;
}

int ROSystem::elementsCount() const {
    int elsCount = 0;
    Q_FOREACH(const ROPass* pass, _passes) {
        elsCount += pass->elementsCount();
    }
    return elsCount;
}

int ROSystem::stagesCount() const {
    int stagesCount = 0;
    Q_FOREACH(const ROPass* pass, _passes) {
        stagesCount += pass->stageCount();
    }
    return stagesCount;
}

bool ROSystem::setFeedCount(int feedCount) {
    if (_MIN_FEEDS_COUNT <= feedCount && feedCount <= _MAX_FEEDS_COUNT) {
        while (feedCount > _feeds.count()) addPartFeed();
        while (feedCount < _feeds.count()) removePartFeed();
        return true;
    }
    return false;
}

int ROSystem::feedCount() const { return _feeds.count(); }

bool ROSystem::removePartFeed(int feedIndex) {
    if (feedIndex == -1) feedIndex = _feeds.count()-1; // last
    if (_feeds.count() > _MIN_FEEDS_COUNT && (0 <= feedIndex && feedIndex < _feeds.count())) {
        ROFeed* removingFeed = _feeds.takeAt(feedIndex);
        _feedsToResultFeed_STH->removeFeed(removingFeed);
        delete removingFeed;
        Q_EMIT feedCountChanged();
        return true;
    }
    return false;
}

ROFlow* const ROSystem::permeate() const { return lastPass()->permeate(); }
void ROSystem::refreshPermeate() { connect(lastPass()->permeate(), SIGNAL(rateChanged()), this, SIGNAL(recoveryChanged())); Q_EMIT permeateChanged();}

void ROSystem::reset() {

    setFeedCount(1);
    firstPartFeed()->reset();

    setWaterTypeIndex(-1);
    scalingElement()->reset();

    resetSystem();
}

void ROSystem::resetSystem() {
    setPassCount(1);
    firstPass()->reset();
    _passRecycles.clear();
}


double ROSystem::recovery() const {
    double inputRate = feed()->rate();
    double outputRate = permeate()->rate();
    return outputRate / inputRate;
}

double ROSystem::power() const {
    double sysPower = 0.0;
    Q_FOREACH(ROPass* p, _passes) {
        sysPower += p->power();
    }
    return sysPower;
}

double ROSystem::specificEnergy() const {
    double sysEnergy = 0.0;
    Q_FOREACH(ROPass* p, _passes) {
        sysEnergy += p->specificEnergy();
    }
    return sysEnergy;
}

double ROSystem::temperature() const { return feed()->temperature(); }
double ROSystem::pH() const { return feed()->pH(); }

void ROSystem::removePassRecycle(int fromPassIdx, int toPassIdx) {
    if (_passRecycles.contains(fromPassIdx)) {
        if (toPassIdx == -1) _passRecycles.remove(fromPassIdx);
        else _passRecycles[fromPassIdx].remove(toPassIdx);
    }
}

void ROSystem::removePassRecycle(const ROPass *const fromPass, const ROPass *const toPass) {
    removePassRecycle(passIndex(fromPass), passIndex(toPass));
}

bool ROSystem::hasRecycle(int fromPassIdx, int toPassIdx) const {
    return (_passRecycles.contains(fromPassIdx) && _passRecycles.value(fromPassIdx).contains(toPassIdx));
}

bool ROSystem::hasRecycle(const ROPass *const fromPass, const ROPass *const toPass) const {
    return hasRecycle(passIndex(fromPass), passIndex(toPass));
}

QVector<int> ROSystem::filledIons() const {
    if (feed()->solutes()->filledIons().count() < adjustedFeed()->solutes()->filledIons().count())
        return adjustedFeed()->solutes()->filledIons();
    else
        return feed()->solutes()->filledIons();
}

QVector<int> ROSystem::saturatedCompounds() const {
    if (feed()->solutes()->saturatedCompounds().count() < adjustedFeed()->solutes()->saturatedCompounds().count())
        return adjustedFeed()->solutes()->saturatedCompounds();
    else
        return feed()->solutes()->saturatedCompounds();
}

double ROSystem::activeArea() const {
    double totalArea = 0.0;
    Q_FOREACH(const ROPass* pass, _passes) {
        totalArea += pass->activeArea();
    }
    return totalArea;
}

int ROSystem::waterTypeIndex() const { return _waterTypeIndex; }
void ROSystem::setWaterTypeIndex(int index) {
    if (index != waterTypeIndex()) {
        _waterTypeIndex = index;
        Q_EMIT waterTypeIndexChanged();
    }
}

ROScalingElement *const ROSystem::scalingElement() const { return _scalingElement; }

double ROSystem::flowFactor() const {
    return roDB->waterTypes()->get(waterTypeIndex(), "flow_factor").toDouble();
}

double ROSystem::permeateFlowFactor() const {
    return roDB->waterTypes()->get(roDB->waterTypes()->WATER_TYPE_PERMEATE, "flow_factor").toDouble();
}

double ROSystem::saltPassageYearIncrease() const
{
    return roDB->waterTypes()->get(waterTypeIndex(), "salt_passage_year_increase").toDouble();
}

double ROSystem::permeateSaltPassageYearIncrease() const
{
    return roDB->waterTypes()->get(roDB->waterTypes()->WATER_TYPE_PERMEATE, "salt_passage_year_increase").toDouble();
}

int ROSystem::totalRecycleCount() const {
    int totalCount = 0;
    Q_FOREACH(const auto& rs, _passRecycles) totalCount += rs.count();
    return totalCount;
}

int ROSystem::lifetime() const {
    return _lifetime;
}

void ROSystem::setLifetime(int lifetime) {
    _lifetime = qBound(1, lifetime, 5);  // TODO consts
    Q_EMIT lifetimeChanged();
}

const QMap<int, double> ROSystem::passIncomingRecycles(int toPassIdx) const {
    QMap<int, double> incomingRecycles;

    auto i = _passRecycles.constBegin();
    while (i != _passRecycles.constEnd()) {
        if (i.value().contains(toPassIdx))
            incomingRecycles.insert(i.key(), i.value().value(toPassIdx));
        ++i;
    }

    return incomingRecycles;
}

const QMap<int, double> ROSystem::passOutgoingRecycles(int fromPassIdx) const {
    return _passRecycles[fromPassIdx];
}

void ROSystem::copyDataFrom(const ROSystem* const from) {
    this->copySystemDataFrom(from);


    this->setWaterTypeIndex(from->waterTypeIndex()); //newPass->setFlowFactor(this->flowFactor());
    // COPY FEEDS
    this->setFeedCount(from->feedCount());
    for (int fIdx = 0; fIdx < from->feedCount(); ++fIdx){
        this->partFeed(fIdx)->copyDataFrom(from->partFeed(fIdx));
    }
}

void ROSystem::copySystemDataFrom(const ROSystem *const from) {
    this->concentrate()->copyDataFrom(from->concentrate());
    this->permeate()->copyDataFrom(from->permeate());

    // COPY PASSES
    this->setPassCount(from->passCount());
    for (int pIdx = 0; pIdx < from->passCount(); ++pIdx){
        this->pass(pIdx)->copyDataFrom(from->pass(pIdx));
        // copy recycles
        for (int pIdx2=0; pIdx2 < pIdx; ++pIdx2){
            if (from->hasRecycle(pIdx, pIdx2)) {
                this->pass(pIdx)->addRecycle(pIdx2, from->recycle(pIdx, pIdx2));
            }
        }
    }
}
