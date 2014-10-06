#include "rostagecontroller.h"

#include "QStringList"

#include <functional>

ROStageController::ROStageController(ROStage* stage, ROPassController* passC) :
    _stage(stage),
    _passC(passC),
    _rawWaterToFeed_SRT(new ROFlowMixer(ROFlowMixer::FlowSolutesTemperature | ROFlowMixer::FlowRate)),
    QObject(stage) {

    _membraneChosen = new ROWarning([this]() { return this->stage()->membraneId() < 0; },
    ROWarning::WarningCritical,
    [this]() { return tr("Membrane element is not chosen"); },
    this);

    _rawWaterToFeed_SRT->addFeed(stage->rawWater(), ROFlowMixer::FlowAdd);
    _rawWaterToFeed_SRT->setOutputFlow(stage->feed());

    connect(stage, SIGNAL(rawWaterChanged()), this, SLOT(updateRawWater()));

    connect(stage, SIGNAL(membraneIdChanged()), _membraneChosen, SLOT(update()));


    connect(_stage, SIGNAL(elementsPerVesselCountChanged()), this, SLOT(updateElements()));
    updateElements();


    // UPDATE WARNINGS STATE
    // ... CRITICAL
    connect(_membraneChosen, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));


    // INPUT CHANGES
    connect(_stage, SIGNAL(membraneIdChanged()), this, SIGNAL(inputChanged()));
    connect(_stage, SIGNAL(elementsPerVesselCountChanged()), this, SIGNAL(inputChanged()));
    connect(_stage, SIGNAL(vesselsCountChanged()), this, SIGNAL(inputChanged()));
    connect(_stage, SIGNAL(rawWaterChanged()), this, SIGNAL(inputChanged()));
    connect(_stage, SIGNAL(preStagePressureChanged()), this, SIGNAL(inputChanged()));
    connect(_stage, SIGNAL(backPressureChanged()), this, SIGNAL(inputChanged()));

    hasAnyCriticalWarningsChanged();
    hasAnyCautionWarningsChanged();
}

ROStageController::ROStageController(): _stage(nullptr), _passC(nullptr), _rawWaterToFeed_SRT(nullptr) { }  // for QML

ROStageController::~ROStageController() {
    delete _membraneChosen;
    //qDeleteAll(_elementControllers); _elementControllers.clear(); - not needed, because of set parent of each element
}

ROStage* const ROStageController::stage() const { return _stage; }

ROWarning* const ROStageController::membraneChosen() const { return _membraneChosen; }

bool ROStageController::hasAnyCriticalWarnings() const {
    bool hasW = _membraneChosen->enabled();
    for (int vcIdx = 0; vcIdx < _elementControllers.count(); ++vcIdx) {
        hasW = hasW || _elementControllers.at(vcIdx)->hasAnyCriticalWarnings();
    }
    return hasW;
}
bool ROStageController::hasAnyCautionWarnings() const {
    bool hasW = false;
    for (int vcIdx = 0; vcIdx < _elementControllers.count(); ++vcIdx) {
        hasW = hasW || _elementControllers.at(vcIdx)->hasAnyCautionWarnings();
    }
    return hasW;
}

QStringList ROStageController::allWarningsMessages(ROWarning::WarningType type) const {
    QStringList messages;
    int stageIndex = passC()->pass()->stageIndex(stage());
    if (type & ROWarning::WarningCritical) {
        if (membraneChosen()->enabled()) messages.append(tr("Stage %1: %2").arg(stageIndex+1).arg(membraneChosen()->what()));
    }
    for (int vcIdx = 0; vcIdx < _elementControllers.count(); ++vcIdx) {
        QStringList elWM = _elementControllers.at(vcIdx)->allWarningsMessages(type);
        Q_FOREACH(const QString& wm, elWM)
            messages.append(tr("Stage %1: %2").arg(stageIndex+1).arg(wm));
    }
    return messages;
}

ROPassController* const ROStageController::passC() const { return _passC; }


ROElementController* ROStageController::elementC(int elementCIndex) const { return _elementControllers.value(elementCIndex); }
ROElementController* ROStageController::elementC(ROElement* element) const { return elementC(stage()->elementIndex(element)); }


void ROStageController::updateElements() {
    if (stage()->elementsPerVesselCount() < _elementControllers.count()) { // element has removed
        int vcIdx = 0;
        while (stage()->elementsPerVesselCount() < _elementControllers.count()) {
            if (stage()->elementIndex(_elementControllers[vcIdx]->element()) == -1)
                _elementControllers.removeAt(vcIdx);
            else
                ++vcIdx;
        }
    } else {
        for (int vcIdx = _elementControllers.count(); vcIdx < stage()->elementsPerVesselCount(); ++vcIdx) { // element has added
            _elementControllers.append(new ROElementController(stage()->element(vcIdx), this));
            connect(_elementControllers.last(), SIGNAL(hasAnyCriticalWarningsChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));
            connect(_elementControllers.last(), SIGNAL(hasAnyCautionWarningsChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
        }
    }
    hasAnyCriticalWarningsChanged();
    hasAnyCautionWarningsChanged();
    inputChanged();
}

void ROStageController::updateRawWater()
{
    _rawWaterToFeed_SRT->clearFeeds();
    _rawWaterToFeed_SRT->addFeed(_stage->rawWater(), ROFlowMixer::FlowAdd);
}

void ROStageController::updateWarnings() {
    _membraneChosen->update();
    Q_FOREACH(ROElementController* ec, _elementControllers) {
        ec->updateWarnings();
    }
}
