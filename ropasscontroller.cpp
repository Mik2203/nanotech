#include "ropasscontroller.h"

#include <QStringList>

#include "ropasssolver.h"

#include "rosystem.h"



ROPassController::ROPassController(ROPass* pass, ROSystemController* sysC) :
    _pass(pass), _sysC(sysC),
    _paramSetC(new ROPassParamSetController()),
    _calculated(false),
    _solved(false),
    _aboutBlend_R(new ROFlowMixer(ROFlowMixer::FlowRate)),
    _toTotalProduct_RSTP(new ROFlowMixer(ROFlowMixer::FlowRate | ROFlowMixer::FlowSolutes | ROFlowMixer::FlowTemperature | ROFlowMixer::FlowPressure)),
    _toBlending_ST(new ROFlowMixer(ROFlowMixer::FlowSolutes | ROFlowMixer::FlowTemperature)),
    _toFeed_ST(new ROFlowMixer(ROFlowMixer::FlowSolutes | ROFlowMixer::FlowTemperature)),
    _incomingRecycles_R(new ROFlow()),
    QObject(pass) {

    connect(_paramSetC, SIGNAL(anySetStateChanged()), this, SLOT(updateFlowParams()));
    connect(_pass->permeate(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
    connect(_pass->feed(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
    connect(_pass, SIGNAL(recoveryChanged()), this, SLOT(updateFlowParams()));
    updateFlowParams();

    connect(_pass, SIGNAL(rawWaterChangeBegan()), this, SLOT(processRawWaterBeforeChanged()));
    connect(_pass, SIGNAL(rawWaterChanged()), this, SLOT(processRawWaterAfterChanged()));

    connect(_pass, SIGNAL(stageCountChanged()), this, SLOT(updateStages()));
    updateStages();

    connect(_pass, SIGNAL(hasBlendPermeateChanged()), this, SLOT(updateBlend()));
    updateBlend();

    // ON ANY PASS PARAM CHANGED
    // ... PASS
    connect(_pass, SIGNAL(stageCountChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(rawWaterChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(flowFactorChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(recycleChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(hasBlendPermeateChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(blendPermeateChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(hasSelfRecycleChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(selfRecycleChanged()), this, SIGNAL(inputChanged()));
    connect(_pass->permeate(), SIGNAL(rateChanged()), this, SIGNAL(inputChanged()));
    connect(_pass->feed(), SIGNAL(rateChanged()), this, SIGNAL(inputChanged()));

    // ... PARAM SETS
    connect(_paramSetC, SIGNAL(anySetStateChanged()), this, SIGNAL(inputChanged()));

    // потоки
    _toTotalProduct_RSTP->addFeed(_pass->permeate(), ROFlowMixer::FlowAdd);
    _toTotalProduct_RSTP->setOutputFlow(pass->totalProduct());

    _toBlending_ST->addFeed(_pass->rawWater(), ROFlowMixer::FlowAdd);
    _toBlending_ST->setOutputFlow(_pass->_blending);

    _toFeed_ST->addFeed(_pass->rawWater(), ROFlowMixer::FlowAdd);
    _toFeed_ST->setOutputFlow(_pass->feed());


    connect(_pass->rawWater(), SIGNAL(rateChanged()), this, SLOT(updateBlendPermeate()));

    connect(_pass, SIGNAL(incomingRecyclesChanged()), this, SLOT(updateRawWater()));


    // WARNINGS
    _fillFlowData = new ROWarning([this]() { return !this->paramSetC()->allSet(); },
    ROWarning::WarningCritical,
   [this]() { return tr("You must complete 2 of 3 values"); },
    this);
    connect(_paramSetC, SIGNAL(anySetStateChanged()), _fillFlowData, SLOT(update()));

    _checkRecovery = new ROWarning([this]() { return this->pass()->recovery() > this->pass()->MAX_RECOVERY() ||
                                   this->pass()->recovery() < this->pass()->MIN_RECOVERY(); },
    ROWarning::WarningCritical,
    [this]() { return tr("Recovery should be more than %2% and less than %1%").arg(_pass->MAX_RECOVERY()*100).arg(_pass->MIN_RECOVERY()*100); },
    this);
    connect(_pass, SIGNAL(recoveryChanged()), _checkRecovery, SLOT(update()));

    // UPDATE WARNINGS STATE
    // ... CRITICAL
    connect(_fillFlowData, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
    connect(_checkRecovery, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
}

ROPassController::ROPassController(): _pass(nullptr), _sysC(nullptr), _paramSetC(nullptr), _incomingRecycles_R(nullptr) { }

ROStageController* ROPassController::stageC(int stageCIndex) const { return _stageControllers.value(stageCIndex); }
ROStageController* ROPassController::stageC(ROStage* stage) const { return stageC(pass()->stageIndex(stage)); }

ROPassParamSetController* const ROPassController::paramSetC() const { return _paramSetC; }
ROPass* const ROPassController::pass() const { return _pass; }

void ROPassController::updateFlowParams() {
    if (_paramSetC->permeateSetState() == ROPassParamSetController::ParamSetImplicit) {
        disconnect(_pass->permeate(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
        _pass->permeate()->setRate(_pass->feed()->rate() * _pass->recovery() );
        connect(_pass->permeate(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
    }
    else if (_paramSetC->feedSetState() == ROPassParamSetController::ParamSetImplicit) {
        disconnect(_pass->feed(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
        _pass->feed()->setRate(_pass->permeate()->rate() / _pass->recovery() );
        connect(_pass->feed(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
    }
    else if (_paramSetC->recoverySetState() == ROPassParamSetController::ParamSetImplicit) {
        disconnect(_pass, SIGNAL(recoveryChanged()), this, SLOT(updateFlowParams()));
        _pass->setRecovery(_pass->permeate()->rate() / _pass->feed()->rate());
        connect(_pass, SIGNAL(recoveryChanged()), this, SLOT(updateFlowParams()));
    }

}

bool ROPassController::hasAnyCriticalWarnings() const {
    bool hasW = _fillFlowData->enabled() || _checkRecovery->enabled() /*|| _badInputData->enabled()*/;
    for (int scIdx = 0; scIdx < _stageControllers.count(); ++scIdx) {
        hasW = hasW || _stageControllers.at(scIdx)->hasAnyCriticalWarnings();
    }
    return hasW;
}

bool ROPassController::hasAnyCautionWarnings() const {
    bool hasW = false;// _checkFlux->enabled() || _checkMaxRecovery->enabled();
    for (int scIdx = 0; scIdx < _stageControllers.count(); ++scIdx) {
        hasW = hasW || _stageControllers.at(scIdx)->hasAnyCautionWarnings();
    }
    return hasW;
}

QStringList ROPassController::allWarningsMessages(ROWarning::WarningType type) const {
    QStringList messages;
    int passIndex = sysC()->sys()->passIndex(pass());
    if (type & ROWarning::WarningCritical) {
        if (fillFlowData()->enabled()) messages.append(tr("Pass %1: %2").arg(passIndex+1).arg(fillFlowData()->what()));
        if (checkRecovery()->enabled()) messages.append(tr("Pass %1: %2").arg(passIndex+1).arg(checkRecovery()->what()));
    }
    for (int scIdx = 0; scIdx < _stageControllers.count(); ++scIdx) {
        QStringList sWM = _stageControllers.at(scIdx)->allWarningsMessages(type);
        Q_FOREACH(const QString& wm, sWM)
            messages.append(tr("Pass %1: %2").arg(passIndex+1).arg(wm));
    }
    return messages;
}


void ROPassController::updateStages() {
    int scIdx = 0;
    while (pass()->stageCount() < _stageControllers.count()) {
        if (pass()->stageIndex(_stageControllers[scIdx]->stage()) == -1)
            _stageControllers.removeAt(scIdx);
        else
            ++scIdx;
    }
    for (int scIdx = _stageControllers.count(); scIdx < pass()->stageCount(); ++scIdx) { // pass has added
        _stageControllers.append(new ROStageController(pass()->stage(scIdx), this));
        connect(_stageControllers.last(), SIGNAL(hasAnyCriticalWarningsChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
        connect(_stageControllers.last(), SIGNAL(hasAnyCautionWarningsChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
        connect(_stageControllers.last(), SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
    }

    emit hasAnyCriticalWarningsChanged();
    emit hasAnyCautionWarningsChanged();
    emit inputChanged();
    // TODO recalc pass
}

void ROPassController::updateBlend()
{
    ROPass * blendPass = pass();

    _aboutBlend_R->reset();


    // totalProduct
    if (blendPass->hasBlendPermeate())
        _toTotalProduct_RSTP->addFeed(_pass->_blending, ROFlowMixer::FlowAdd);
    else
        _toTotalProduct_RSTP->removeFeed(_pass->_blending);


    // feed|blend|raw
    if (blendPass == pass()->system()->firstPass()) {
        // смешение на 1 ступени - модель, когда feed и blending являются независимыми,
        // а объем входного потока rawWater является зависимым и увеличивается.
        _aboutBlend_R->addFeed(blendPass->feed(), ROFlowMixer::FlowAdd);
        _aboutBlend_R->addFeed(_incomingRecycles_R, ROFlowMixer::FlowSubtract);
        _aboutBlend_R->setOutputFlow(blendPass->rawWater());

        if (blendPass->hasBlendPermeate())
            _aboutBlend_R->addFeed(blendPass->_blending, ROFlowMixer::FlowAdd);
    } else {
        // модель когда feed является зависимым, blending, если есть, ограничивается,
        // а rawWater - независим.
        _aboutBlend_R->addFeed(blendPass->rawWater(), ROFlowMixer::FlowAdd);
        _aboutBlend_R->setOutputFlow(blendPass->feed());

        if (blendPass->hasBlendPermeate())
            _aboutBlend_R->addFeed(blendPass->_blending, ROFlowMixer::FlowSubtract);
    }
}

void ROPassController::processRawWaterBeforeChanged()
{
    disconnect(_pass->rawWater(), 0, this, 0);
    _aboutBlend_R->reset();
    _toBlending_ST->removeFeed(_pass->rawWater());
    _toFeed_ST->removeFeed(_pass->rawWater());
}

void ROPassController::processRawWaterAfterChanged()
{
    connect(_pass->rawWater(), SIGNAL(rateChanged()), this, SLOT(updateBlendPermeate()));
    updateBlend();
    _toBlending_ST->addFeed(_pass->rawWater(), ROFlowMixer::FlowAdd);
    _toFeed_ST->addFeed(_pass->rawWater(), ROFlowMixer::FlowAdd);
}



ROWarning* const ROPassController::fillFlowData() const { return _fillFlowData; }
ROWarning* const ROPassController::checkRecovery() const { return _checkRecovery; }

ROSystemController* const ROPassController::sysC() const { return _sysC; }


ROPassController::~ROPassController() {
    delete _paramSetC;

    // WARNINGS
    delete _fillFlowData;
    delete _checkRecovery;

    // mixers
    delete _aboutBlend_R;
    delete _toTotalProduct_RSTP;
    delete _toBlending_ST;
    delete _toFeed_ST;
}

void ROPassController::copyDataFrom(const ROPassController *const from) {
    this->paramSetC()->copyDataFrom(from->paramSetC());
}


void ROPassController::updateWarnings() {
    _fillFlowData->update();
    _checkRecovery->update();
    Q_FOREACH(ROStageController* sc, _stageControllers) {
        sc->updateWarnings();
    }
}

void ROPassController::reset() {
    paramSetC()->reset();
}


// пересчет значения смешения при изменении входной воды (rawWater)
// для учета ограничений в setBlendPermeate.
// если ступень - первая, то смешение не ограничевается, т.к. не зависит от входного потока.
void ROPassController::updateBlendPermeate() {
    if (_pass->hasBlendPermeate() && _pass != _pass->system()->firstPass())
        _pass->setBlendPermeate(_pass->_blending->rate());
}

void ROPassController::updateRawWater()
{
    _incomingRecycles_R->setRate(pass()->incomingRecyclesRate());
}
