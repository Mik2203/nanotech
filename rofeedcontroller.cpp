#include "rofeedcontroller.h"

#include <QStringList>

ROFeedController::ROFeedController(ROFlow* feed, ROSystemController* sysC) :
    _feed(feed), _sysC(sysC),
    QObject(feed)
{
        // WARNINGS
        _ionsFilled = new ROWarning([this]() { return !this->feed()->solutes()->isFilled(); },
    ROWarning::WarningCritical,
    [this]() { return tr("You must fill values of ions"); },
    this);
    connect(_feed->solutes(), SIGNAL(solutesChanged()), _ionsFilled, SLOT(update()));

    _ionsBalanced = new ROWarning([this]() { return !this->feed()->solutes()->isBalanced(); },
    ROWarning::WarningCaution,
    [this]() { return tr("Cations and anions are not balanced"); },
    this);
    connect(_feed->solutes(), SIGNAL(solutesChanged()), _ionsBalanced, SLOT(update()));

    // UPDATE WARNINGS STATE
    // ... CRITICAL
    connect(_ionsFilled, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCriticalWarningsChanged()));

    // ... CAUTION
    connect(_ionsBalanced, SIGNAL(enabledChanged()), this, SIGNAL(hasAnyCautionWarningsChanged()));





    // INPUT CHANGED
    connect(_feed, SIGNAL(rateChanged()), this, SIGNAL(inputChanged()));
    connect(_feed, SIGNAL(pHChanged()), this, SIGNAL(inputChanged()));
    connect(_feed, SIGNAL(solutesChanged()), this, SIGNAL(inputChanged()));
    connect(_feed, SIGNAL(temperatureChanged()), this, SIGNAL(inputChanged()));


    // UPDATE

    hasAnyCriticalWarningsChanged();
    hasAnyCautionWarningsChanged();
}

ROFeedController::ROFeedController(): _feed(0), _sysC(0) {}

void ROFeedController::copyDataFrom(const ROFeedController *const from) { /*YET NOTHING TO COPY*/ }

ROFlow* const ROFeedController::feed() const { return _feed; }
ROWarning* const ROFeedController::ionsBalanced() const { return _ionsBalanced; }
ROWarning* const ROFeedController::ionsFilled() const { return _ionsFilled; }

bool ROFeedController::hasAnyCriticalWarnings() const { return _ionsFilled->enabled(); }
bool ROFeedController::hasAnyCautionWarnings() const { return _ionsBalanced->enabled(); }

QStringList ROFeedController::allWarningsMessages(ROWarning::WarningType type) const {
    QStringList messages;
    int feedIndex = sysC()->sys()->partFeedIndex(feed());
    if (type & ROWarning::WarningCritical) {
        if (ionsFilled()->enabled()) messages.append(tr("Feed %1: %2").arg(feedIndex+1).arg(ionsFilled()->what()));
    }
    if (type & ROWarning::WarningCaution) {
        if (ionsBalanced()->enabled()) messages.append(tr("Feed %1: %2").arg(feedIndex+1).arg(ionsBalanced()->what()));
    }
    return messages;
}

ROSystemController *const ROFeedController::sysC() const  {return _sysC; }


void ROFeedController::updateWarnings() {
    _ionsBalanced->update();
    _ionsFilled->update();
}
