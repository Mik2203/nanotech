#include "rosystemcontroller.h"

#include <QStringList>

#include "ropass.h"
#include "rostage.h"
#include "rosystemsolvescheduler.h"
#include "roapplication.h"

#ifdef QT_DEBUG
#include <iostream>
#endif


ROSystemController::ROSystemController() {;}

ROSystemController::~ROSystemController() {
    delete _waterTypeChosen;
    delete _badInputData;
}

ROSystemController::ROSystemController(ROSystem* sys, ROSystemSolveScheduler* sysSS, QObject *parent) :
    QObject(parent),
    _sys(sys),
    _sysSS(sysSS){



    connect(_sys, SIGNAL(passCountChanged()), this, SLOT(updatePasses()));
    connect(_sys, SIGNAL(feedCountChanged()), this, SLOT(updateFeeds()));
    updatePasses();
    updateFeeds();

    // RESET CALCULATED
    _waterTypeChosen = new ROWarning([this]() { return this->sys()->waterTypeIndex() == -1; },
    ROWarning::WarningCritical,
    [this]() { return tr("Water type must be chosen"); },
    this);
    connect(_sys, SIGNAL(waterTypeIndexChanged()), _waterTypeChosen, SLOT(update()));

    _badInputData = new ROWarning([this]() { return this->sysSS()->calculated() && !this->sysSS()->solved(); },
    ROWarning::WarningCritical,
    [this]() {  return tr("System can not be calculated. Please change your system design to reduce recovery or permeate flow."); },
    this);
    connect(this->sysSS(), SIGNAL(statusChanged()), _badInputData, SLOT(update()));

    // UPDATE WARNINGS STATE
    // ... CRITICAL
    connect(_waterTypeChosen, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
    connect(_badInputData, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));

    // INPUT CHANGES
    connect(_sys, SIGNAL(feedCountChanged()), this, SIGNAL(inputChanged()));
    connect(_sys, SIGNAL(passCountChanged()), this, SIGNAL(inputChanged()));
    connect(_sys, SIGNAL(waterTypeIndexChanged()), this, SIGNAL(inputChanged()));
    connect(_sys, SIGNAL(elementLifetimeChanged()), this, SIGNAL(inputChanged()));

    // Translation
    connect(roApp->translator(), SIGNAL(currentLanguageChanged()), this, SLOT(updateWarnings()));
}



ROPassController* ROSystemController::passC(int passCIndex) const { return _passControllers.value(passCIndex); }
ROPassController* ROSystemController::passC(ROPass* pass) const { return passC(_sys->passIndex(pass)); }

ROFeedController* ROSystemController::feedC(int feedCIndex) const { return _feedControllers.value(feedCIndex); }
ROFeedController* ROSystemController::feedC(ROFeed* feed) const { return feedC(_sys->partFeedIndex(feed)); }


ROSystem* const ROSystemController::sys() const { return _sys; }

void ROSystemController::updateWarnings() {
    _waterTypeChosen->update();
    _badInputData->update();
    Q_FOREACH(ROPassController* pc, _passControllers) {
        pc->updateWarnings();
    }
    Q_FOREACH(ROFeedController* fc, _feedControllers) {
        fc->updateWarnings();
    }
}

void ROSystemController::reset() {
    Q_FOREACH(ROPassController* pc, _passControllers) pc->reset();
}


void ROSystemController::updatePasses() {
    // TODO beauty
    if (_sys->passCount() < _passControllers.count()) { // pass has been removed
        for (int pcIdx = 0; pcIdx < _passControllers.count(); ++pcIdx) {
            if (_sys->passIndex(_passControllers[pcIdx]->pass()) == -1) { // this pass has been removed
                _passControllers.removeAt(pcIdx);
                if (pcIdx == 0) {
                    _passControllers.first()->paramSetC()->setFeedSetState(ROPassParamSetController::ParamSetExplicit);
//                    _passControllers.first()->paramSetC()->setPrevController(0);
                } else if (pcIdx < _passControllers.count()) {
//                    _passControllers[pcIdx]->paramSetC()->setPrevController(_passControllers[pcIdx-1]->paramSetC());
                } else if (pcIdx == _passControllers.count()) {
//                    _passControllers.last()->paramSetC()->setNextController(0);
                }
                if (_sys->passCount() >= _passControllers.count()) break;
            }
        }
    } else {
        while (_sys->passCount() > _passControllers.count()) { // pass has been added
            ROPassController* addedPC = new ROPassController(_sys->lastPass(), this);
            connect(addedPC, SIGNAL(hasAnyCriticalWarningsChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
            connect(addedPC, SIGNAL(hasAnyCautionWarningsChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
            if (_passControllers.count() > 0) {
                addedPC->paramSetC()->setFeedSetState(ROPassParamSetController::ParamSetAuto);
            }
            _passControllers.append(addedPC);

            connect(_passControllers.last(), SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
        }
    }
    Q_EMIT hasAnyCriticalWarningsChanged();
    Q_EMIT hasAnyCautionWarningsChanged();
    // TODO recalc - если делать неявный апдейт системы, а не по кнопке calculate
}


void ROSystemController::updateFeeds() {
    // TODO beauty
    if (sys()->feedCount() < _feedControllers.count()) { // feed has removed
        for (int fcIdx = 0; fcIdx < _feedControllers.count(); ++fcIdx) {
            if (sys()->partFeedIndex(_feedControllers[fcIdx]->feed()) == -1) {// this feed has removed
                _feedControllers.removeAt(fcIdx);
                if (sys()->feedCount() >= _feedControllers.count()) break;
            }
        }
    } else {
        while (sys()->feedCount() > _feedControllers.count()) { // feed has added
            _feedControllers.append(new ROFeedController(sys()->lastPartFeed(), this));
            connect(_feedControllers.last(), SIGNAL(hasAnyCriticalWarningsChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
            connect(_feedControllers.last(), SIGNAL(hasAnyCautionWarningsChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
            connect(_feedControllers.last(), SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
        }
    }
    Q_EMIT hasAnyCriticalWarningsChanged();
    Q_EMIT hasAnyCautionWarningsChanged();
    // TODO recalc ?
}

ROWarning* const ROSystemController::waterTypeChosen() const { return _waterTypeChosen; }
ROWarning* const ROSystemController::badInputData() const { return _badInputData; }

QStringList ROSystemController::allWarningsMessages(unsigned type) const {
    QStringList messages;
    if (type & ROWarning::WarningCritical) {
        if (waterTypeChosen()->enabled()) messages.append(tr("System: %1").arg(waterTypeChosen()->what()));
        if (badInputData()->enabled()) messages.append(tr("System: %1").arg(badInputData()->what()));
    }

    for (int fcIdx = 0; fcIdx < _feedControllers.count(); ++fcIdx) {
        QStringList fWM = _feedControllers.at(fcIdx)->allWarningsMessages(static_cast<ROWarning::WarningType>(type));
        Q_FOREACH(const QString& wm, fWM)
            messages.append(tr("System: %1").arg(wm));
    }

    for (int pcIdx = 0; pcIdx < _passControllers.count(); ++pcIdx) {
        QStringList pWM = _passControllers.at(pcIdx)->allWarningsMessages(static_cast<ROWarning::WarningType>(type));
        Q_FOREACH(const QString& wm, pWM)
            messages.append(tr("System: %1").arg(wm));
    }

    return messages;
}

bool ROSystemController::hasAnyCriticalWarnings() const {
    bool hasW = _waterTypeChosen->enabled() || _badInputData->enabled();
    for (int pcIdx = 0; pcIdx < _passControllers.count(); ++pcIdx) {
        hasW = hasW || _passControllers.at(pcIdx)->hasAnyCriticalWarnings();
    }
    for (int fcIdx = 0; fcIdx < _feedControllers.count(); ++fcIdx) {
        hasW = hasW || _feedControllers.at(fcIdx)->hasAnyCriticalWarnings();
    }
    return hasW;
}

bool ROSystemController::hasAnyCautionWarnings() const {
    bool hasW = false;
    for (int pcIdx = 0; pcIdx < _passControllers.count(); ++pcIdx) {
        hasW = hasW || _passControllers.at(pcIdx)->hasAnyCautionWarnings();
    }
    for (int fcIdx = 0; fcIdx < _feedControllers.count(); ++fcIdx) {
        hasW = hasW || _feedControllers.at(fcIdx)->hasAnyCautionWarnings();
    }
    return hasW;
}


void ROSystemController::setSys(ROSystem* const newSys) {
    // TODO: setPass(), not delete and create

    // disconnect old sys
    disconnect(_sys, 0, this, 0);
    disconnect(_sys, 0, _waterTypeChosen, 0);
    qDeleteAll(_passControllers); _passControllers.clear();

    _sys = newSys;

    // connect new sys
    connect(_sys, SIGNAL(waterTypeIndexChanged()), _waterTypeChosen, SLOT(update()));
    _waterTypeChosen->update();
    connect(_sys, SIGNAL(passCountChanged()), this, SLOT(updatePasses()));
    connect(_sys, SIGNAL(feedCountChanged()), this, SLOT(updateFeeds()));
    updatePasses();
    updateFeeds();
}


void ROSystemController::copyDataFrom(const ROSystemController* const from) {
    // COPY PASS CONTROLLERS
    int pcCount = qMin(this->passCCount(), from->passCCount());
    for (int pcIdx = 0; pcIdx < pcCount; ++pcIdx){
        this->passC(pcIdx)->copyDataFrom(from->passC(pcIdx));
    }

    // COPY FEED CONTROLLERS
    int fcCount = qMin(this->feedCCount(), from->feedCCount());
    for (int fcIdx = 0; fcIdx < fcCount; ++fcIdx){
        this->feedC(fcIdx)->copyDataFrom(from->feedC(fcIdx));
    }
}

ROSystemSolveScheduler *const ROSystemController::sysSS() const { return _sysSS; }

int ROSystemController::passCCount() const { return _passControllers.count(); }
int ROSystemController::feedCCount() const { return _feedControllers.count(); }
