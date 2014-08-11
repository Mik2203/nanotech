#include "roelementcontroller.h"
#include "rostage.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif
#include <QStringList>

#include <functional>
#include "rostagecontroller.h"
#include "romath.h"
#include "rodatabase.h"
#include "rounits.h"
#include "rounitstext.h"

ROElementController::ROElementController(ROElement *element, ROStageController* stageC):
    _element(element), _stageC(stageC), QObject(element) {


    _checkMaxFeedPressure = new ROWarning(
                [this]() { return this->stageC()->passC()->sysC()->sysSS()->solved() &&
                                          //this->checkVar(this->element()->stage()->pass()->system()->MEMBRANES_MODEL()->get(this->element()->stage()->membraneIndex(), "max_feed_pressure").toDouble()) &&
                                          (
                                              roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "max_feed_pressure").toDouble() < this->element()->feed()->pressure()); },
    ROWarning::WarningCaution,
    [this]() -> QString {
        double maxFeedPressureBar = roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "max_feed_pressure").toDouble();
        double maxFeedPressure = ROUnits::convertPressureUnits(maxFeedPressureBar, ROUnits::bar, roUnits->pressureUnits());
        return tr("Feed pressure should be less than %1 %2").arg(QString::number(maxFeedPressure, 'f', 2)).arg(roUnitsText->pressureUnitText(roUnits->pressureUnits()));
    },
    this);

//    connect(_element->feed(), SIGNAL(pressureChanged()), _checkMaxFeedPressure, SLOT(update()));
    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkMaxFeedPressure, SLOT(update()));
    connect(roUnits, SIGNAL(pressureUnitsChanged()), _checkMaxFeedPressure, SLOT(update()));

    _checkMaxPressureDrop = new ROWarning([this]() -> bool {
            if (!this->stageC()->passC()->sysC()->sysSS()->solved()) return false;
            double maxDrop = roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "max_pressure_drop").toDouble();
            double curDrop = dPfc(this->element()->feed()->rate(), this->element()->concentrate()->rate());
            return curDrop > maxDrop;
    },
    ROWarning::WarningCaution,
    [this]() -> QString {
        double maxPressureDropBar = roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "max_pressure_drop").toDouble();
        double maxPressureDrop = ROUnits::convertPressureUnits(maxPressureDropBar, ROUnits::bar, roUnits->pressureUnits());
        return tr("Pressure drop should be less than %1 %2").arg(QString::number(maxPressureDrop, 'f', 2)).arg(roUnitsText->pressureUnitText(roUnits->pressureUnits()));
    },
    this);

    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkMaxPressureDrop, SLOT(update()));
    connect(roUnits, SIGNAL(pressureUnitsChanged()), _checkMaxPressureDrop, SLOT(update()));


    _checkMaxFeedRate = new ROWarning([this]() { return this->stageC()->passC()->sysC()->sysSS()->solved() &&
                                              roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "max_feed_rate").toDouble() < this->element()->feed()->rate(); },
    ROWarning::WarningCaution,
    [this]() -> QString {
        double maxFeedRateM3h = roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "max_feed_rate").toDouble();
        double maxFeedRate = ROUnits::convertFlowUnits(maxFeedRateM3h, ROUnits::m3h, roUnits->flowUnits());
        return tr("Feed rate should be less than %1 %2").arg(QString::number(maxFeedRate, 'f', 2)).arg(roUnitsText->flowUnitText(roUnits->flowUnits()));
    },
    this);

    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkMaxFeedRate, SLOT(update()));
    connect(roUnits, SIGNAL(flowUnitsChanged()), _checkMaxFeedRate, SLOT(update()));


    _checkMinConcentrate = new ROWarning(
           [this]() -> bool {
                if (!this->stageC()->passC()->sysC()->sysSS()->solved()) return false;
                int waterTypeIndex = this->element()->stage()->pass()->system()->passIndex(this->element()->stage()->pass()) > 0 ? 0 : this->element()->stage()->pass()->system()->waterTypeIndex();
                double minConcentrate = roDB->waterTypes()->get(waterTypeIndex, "min_concentrate").toDouble();
                double membraneArea = roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "area").toDouble();
                if (membraneArea > 30.0) ; // 8040
                else if (membraneArea > 7.0) minConcentrate /= 4; // 4040
                else if (membraneArea > 2.0) minConcentrate /= 8; // 2540
                else if (membraneArea > 1.0) minConcentrate /= 16; // 2521
                else minConcentrate /= 32; // 1812-15
                return minConcentrate > this->element()->concentrate()->rate();
            },
    ROWarning::WarningCaution,
    [this]() -> QString {
        if (!this->stageC()->passC()->sysC()->sysSS()->solved()) return false;
        int waterTypeIndex = this->element()->stage()->pass()->system()->passIndex(this->element()->stage()->pass()) > 0 ? 0 : this->element()->stage()->pass()->system()->waterTypeIndex();
        double minConcentrateM3h = roDB->waterTypes()->get(waterTypeIndex, "min_concentrate").toDouble();

        double membraneArea = roDB->membranes()->get(roDB->membranes()->indexById(this->element()->stage()->membraneId()), "area").toDouble();
        if (membraneArea > 30.0) ; // 8040
        else if (membraneArea > 7.0) minConcentrateM3h /= 4; // 4040
        else if (membraneArea > 2.0) minConcentrateM3h /= 8; // 2540
        else if (membraneArea > 1.0) minConcentrateM3h /= 16; // 2521
        else minConcentrateM3h /= 32; // 1812-15

        double minConcentrate = ROUnits::convertFlowUnits(minConcentrateM3h, ROUnits::m3h, roUnits->flowUnits());

        return tr("Concentrate rate should be more than %1 %2").arg(QString::number(minConcentrate, 'f', 2)).arg(roUnitsText->flowUnitText(roUnits->flowUnits()));
    },
    this);

    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkMinConcentrate, SLOT(update()));
    connect(_element->stage()->pass()->system(), SIGNAL(waterTypeIndexChanged()), _checkMinConcentrate, SLOT(update()));
    connect(roUnits, SIGNAL(flowUnitsChanged()), _checkMinConcentrate, SLOT(update()));

    _checkFlux = new ROWarning([this]() -> bool {
        int waterTypeIndex = this->element()->stage()->pass()->system()->passIndex(this->element()->stage()->pass()) > 0 ? 0 : this->element()->stage()->pass()->system()->waterTypeIndex();
        return (this->stageC()->passC()->sysC()->sysSS()->solved() && (roDB->waterTypes()->get(waterTypeIndex, "max_flux").toDouble() < this->element()->averageFlux())) ;
            },
    ROWarning::WarningCaution,
    [this]() -> QString {
        int waterTypeIndex = this->element()->stage()->pass()->system()->passIndex(this->element()->stage()->pass()) > 0 ? 0 : this->element()->stage()->pass()->system()->waterTypeIndex();
        double maxFluxLmh = roDB->waterTypes()->get(waterTypeIndex, "max_flux").toDouble();
        double maxFlux = ROUnits::convertFluxUnits(maxFluxLmh, ROUnits::lmh, roUnits->fluxUnits());
        return tr("Average flux should be less than %1 %2").arg(QString::number(maxFlux, 'f', 2)).arg(roUnitsText->fluxUnitText(roUnits->fluxUnits()));
    },
    this);
    connect(_element->stage()->pass()->system(), SIGNAL(waterTypeIndexChanged()), _checkFlux, SLOT(update()));
//    connect(_element, SIGNAL(averageFluxChanged()), _checkFlux, SLOT(update()));
    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkFlux, SLOT(update()));
    connect(roUnits, SIGNAL(fluxUnitsChanged()), _checkFlux, SLOT(update()));

    _checkMaxRecovery = new ROWarning([this]() -> bool {
        int waterTypeIndex = this->element()->stage()->pass()->system()->passIndex(this->element()->stage()->pass()) > 0 ? 0 : this->element()->stage()->pass()->system()->waterTypeIndex();
        return (this->stageC()->passC()->sysC()->sysSS()->solved() &&
               roDB->waterTypes()->get(waterTypeIndex, "max_recovery").toDouble() < this->element()->recovery());
            },
    ROWarning::WarningCaution,
    [this]() -> QString {
        int waterTypeIndex = this->element()->stage()->pass()->system()->passIndex(this->element()->stage()->pass()) > 0 ? 0 : this->element()->stage()->pass()->system()->waterTypeIndex();
        return tr("Recovery should be less than %1%")
                .arg(roDB->waterTypes()->get(waterTypeIndex, "max_recovery").toDouble() * 100.0); },
    this);
    connect(_element->stage()->pass()->system(), SIGNAL(waterTypeIndexChanged()), _checkMaxRecovery, SLOT(update()));
//    connect(_element, SIGNAL(recoveryChanged()), _checkMaxRecovery, SLOT(update()));
    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkMaxRecovery, SLOT(update()));


    _checkLSI = new ROWarning([this]() {return (this->stageC()->passC()->sysC()->sysSS()->solved() &&
                                                this->element()->concentrate()->solutes()->lsi() > 0.0);
            },
    ROWarning::WarningCaution,
    [this]() { return tr("Concentrate LSI > 0. It is necessary to add acid and/or antiscalant in inlet water"); },
    this);
    connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkLSI, SLOT(update()));

    _checkSDSI = new ROWarning([this]() {return (this->stageC()->passC()->sysC()->sysSS()->solved() &&
                                                 this->element()->concentrate()->solutes()->sdsi() > 0.0);
             },
     ROWarning::WarningCaution,
     [this]() { return tr("Concentrate S&DSI > 0. It is necessary to add acid and/or antiscalant in inlet water"); },
     this);
     connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), _checkSDSI, SLOT(update()));

    for (int sati=0; sati<ROSolutes::TotalCompounds; ++sati) {
        ROWarning* checkCompoundSaturation = new ROWarning(
        [&, sati]() -> bool {
            double saturation = this->element()->concentrate()->solutes()->saturation(static_cast<ROSolutes::ScalingCompound>(sati));
            return (this->stageC()->passC()->sysC()->sysSS()->solved() &&
                   saturation >= 0.8);
                },
        ROWarning::WarningCaution,
        [&, sati]() -> QString {
            double saturation = this->element()->concentrate()->solutes()->saturation(static_cast<ROSolutes::ScalingCompound>(sati));
            QString dangerLevel = saturation < 1.0 ? tr("possible") : tr("danger");
            return tr("%1 saturation in concentrate is %2%: %3 scaling, antiscalants may be required.")
                    .arg(ROSoluteModel::shortCompoundNameByIndex(sati)).arg(saturation * 100.0).arg(dangerLevel);
        },
        this);


        connect(_element->concentrate(), SIGNAL(solutesChanged()), checkCompoundSaturation, SLOT(update()));
        connect(_stageC->passC()->sysC()->sysSS(), SIGNAL(statusChanged()), checkCompoundSaturation, SLOT(update()));



        _checkSaturation << checkCompoundSaturation;
        connect(checkCompoundSaturation, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    }


    // UPDATE WARNINGS STATE
    // ... CAUTION
    connect(_checkMaxFeedPressure, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkMaxPressureDrop, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkMaxFeedRate, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkMinConcentrate, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkFlux, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkMaxRecovery, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkLSI, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));
    connect(_checkSDSI, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));


    Q_EMIT hasAnyCriticalWarningsChanged();
    Q_EMIT hasAnyCautionWarningsChanged();
}



ROElementController::ROElementController(): _element(0), _stageC(0) { }

ROElementController::~ROElementController() {
    delete _checkMaxFeedPressure;
    delete _checkMaxPressureDrop;
    delete _checkMaxFeedRate;
    delete _checkMinConcentrate;
    delete _checkFlux;
    delete _checkMaxRecovery;
    delete _checkLSI;
    delete _checkSDSI;
    qDeleteAll(_checkSaturation); _checkSaturation.clear();
}

ROElement* const ROElementController::element() const { return _element; }

ROWarning* const ROElementController::checkMaxFeedPressure() const { return _checkMaxFeedPressure; }
ROWarning* const ROElementController::checkMaxPressureDrop() const { return _checkMaxPressureDrop; }
ROWarning* const ROElementController::checkMaxFeedRate() const { return _checkMaxFeedRate; }
ROWarning* const ROElementController::checkMinConcentrate() const { return _checkMinConcentrate; }
ROWarning* const ROElementController::checkFlux() const { return _checkFlux; }
ROWarning* const ROElementController::checkMaxRecovery() const { return _checkMaxRecovery; }
ROWarning *const ROElementController::checkLSI() const { return _checkLSI; }
ROWarning *const ROElementController::checkSDSI() const { return _checkSDSI; }

ROWarning * ROElementController::checkSaturation(int compound) const {
    return _checkSaturation.value(compound);
}

bool ROElementController::hasAnyCriticalWarnings() const { return false; }
bool ROElementController::hasAnyCautionWarnings() const {
    bool has = (_checkMaxFeedPressure->enabled() ||
            _checkMaxPressureDrop->enabled() ||
            _checkMaxFeedRate->enabled() ||
            _checkMinConcentrate->enabled() ||
            _checkFlux->enabled() ||
            _checkMaxRecovery->enabled() ||
            _checkSDSI->enabled() ||
            _checkLSI->enabled());
    Q_FOREACH(ROWarning* w, _checkSaturation) {
        has = has || w->enabled();
    }
    return has;
}

QStringList ROElementController::allWarningsMessages(ROWarning::WarningType type) const {
    QStringList messages;
    int elementIndex = stageC()->stage()->elementIndex(element());
    if (type & ROWarning::WarningCaution) {
        if (checkMaxFeedPressure()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkMaxFeedPressure()->what()));
        if (checkMaxPressureDrop()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkMaxPressureDrop()->what()));
        if (checkMaxFeedRate()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkMaxFeedRate()->what()));
        if (checkMinConcentrate()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkMinConcentrate()->what()));
        if (checkFlux()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkFlux()->what()));
        if (checkMaxRecovery()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkMaxRecovery()->what()));
        if (checkLSI()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkLSI()->what()));
        if (checkSDSI()->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(checkSDSI()->what()));

        Q_FOREACH(ROWarning* w, _checkSaturation) {
            if (w->enabled()) messages.append(tr("Element %1: %2").arg(elementIndex+1).arg(w->what()));
        }
    }
    return messages;
}

ROStageController* const ROElementController::stageC() const { return _stageC; }

void ROElementController::updateWarnings() {
     _checkMaxFeedPressure->update();
     _checkMaxPressureDrop->update();
     _checkMaxFeedRate->update();
     _checkMinConcentrate->update();
     _checkFlux->update();
     _checkMaxRecovery->update();
     _checkLSI->update();
     _checkSDSI->update();

     Q_FOREACH(ROWarning* w, _checkSaturation) {
         w->update();
     }
}
