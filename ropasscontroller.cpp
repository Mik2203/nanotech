#include "ropasscontroller.h"

#include <QStringList>

#include "ropasssolver.h"

#include "rosystem.h"



ROPassController::ROPassController(ROPass* pass, ROSystemController* sysC) :
    _pass(pass), _sysC(sysC),
    _paramSetC(new ROPassParamSetController()),
    _calculated(false),
    _solved(false),
    QObject(pass) {

    connect(_paramSetC, SIGNAL(anySetStateChanged()), this, SLOT(updateFlowParams()));
    connect(_pass->permeate(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));
    connect(_pass, SIGNAL(recoveryChanged()), this, SLOT(updateFlowParams()));
    connect(_pass, SIGNAL(beginFeedChange()), this, SLOT(disconnectFeed()));
    connect(_pass, SIGNAL(feedChanged()), this, SLOT(updateFeed()));
    updateFeed();

    connect(_pass, SIGNAL(stageCountChanged()), this, SLOT(updateStages()));
    updateStages();

    // ON ANY PASS PARAM CHANGED
    // ... PASS
    connect(_pass, SIGNAL(stageCountChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(feedChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(flowFactorChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(recycleChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(hasBlendPermeateChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(blendPermeateChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(hasSelfRecycleChanged()), this, SIGNAL(inputChanged()));
    connect(_pass, SIGNAL(selfRecycleChanged()), this, SIGNAL(inputChanged()));
    connect(_pass->permeate(), SIGNAL(rateChanged()), this, SIGNAL(inputChanged()));

    // ... PARAM SETS
    connect(_paramSetC, SIGNAL(anySetStateChanged()), this, SIGNAL(inputChanged()));


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

    ROPassController::ROPassController(): _pass(0), _sysC(0), _paramSetC(0) { }

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

void ROPassController::updateFeed() {
    if (_pass->feed()) {
        connect(_pass->feed(), SIGNAL(rateChanged()), this, SLOT(updateFlowParams()));

        // FEED INPUT CHANGES
        connect(_pass->feed(), SIGNAL(rateChanged()), this, SIGNAL(inputChanged()));
        updateFlowParams();
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



ROWarning* const ROPassController::fillFlowData() const { return _fillFlowData; }
ROWarning* const ROPassController::checkRecovery() const { return _checkRecovery; }

ROSystemController* const ROPassController::sysC() const { return _sysC; }


ROPassController::~ROPassController() {
    delete _paramSetC;

    // WARNINGS
    delete _fillFlowData;
    delete _checkRecovery;
}

void ROPassController::copyDataFrom(const ROPassController *const from) {
    this->paramSetC()->copyDataFrom(from->paramSetC());
}

void ROPassController::disconnectFeed() { disconnect(_pass->feed(), 0, this, 0); }


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
