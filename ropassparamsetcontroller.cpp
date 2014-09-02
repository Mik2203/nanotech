#include "ropassparamsetcontroller.h"

ROPassParamSetController::ROPassParamSetController(QObject *parent) :
    //SET STATES
    _feedSetState(ParamSetUndefined),
    _recoverySetState(ParamSetUndefined),
    _permeateSetState(ParamSetUndefined),
    _statesChanging(false),
    QObject(parent)
{

    connect(this, SIGNAL(feedSetStateChanged()), this, SIGNAL(anySetStateChanged()));
    connect(this, SIGNAL(permeateSetStateChanged()), this, SIGNAL(anySetStateChanged()));
    connect(this, SIGNAL(recoverySetStateChanged()), this, SIGNAL(anySetStateChanged()));
}

void ROPassParamSetController::copyDataFrom(const ROPassParamSetController *const from) {
    if (from->feedSetState() == ROPassParamSetController::ParamSetExplicit)
        this->setFeedSetState(from->feedSetState());
    if (from->permeateSetState() == ROPassParamSetController::ParamSetExplicit)
        this->setPermeateSetState(from->permeateSetState());
    if (from->recoverySetState() == ROPassParamSetController::ParamSetExplicit)
        this->setRecoverySetState(from->recoverySetState());
}

ROPassParamSetController::ParamSetState ROPassParamSetController::feedSetState() const { return _feedSetState; }
ROPassParamSetController::ParamSetState ROPassParamSetController::recoverySetState() const { return _recoverySetState; }
ROPassParamSetController::ParamSetState ROPassParamSetController::permeateSetState() const { return _permeateSetState; }


bool ROPassParamSetController::allSet() const {
    return ((feedSetState() != ParamSetUndefined) &&
            (recoverySetState() != ParamSetUndefined) &&
            (permeateSetState() != ParamSetUndefined));

}

void ROPassParamSetController::setFeedSetState(ParamSetState state) { // TODO CHECK!
    if (_feedSetState == state)
        return;

    _feedSetState = state;
    if (state == ParamSetExplicit || state == ParamSetImplicit || state == ParamSetAuto) {
        if (permeateSetState() == ParamSetUndefined &&
                (recoverySetState() == ParamSetExplicit ||
                 recoverySetState() == ParamSetImplicit ||
                 recoverySetState() == ParamSetAuto)) {
            setPermeateSetState(ParamSetImplicit);
        } else if (recoverySetState() == ParamSetUndefined &&
                   (permeateSetState() == ParamSetExplicit ||
                    permeateSetState() == ParamSetImplicit ||
                    permeateSetState() == ParamSetAuto))  {
            setRecoverySetState(ParamSetImplicit);
        }
    } else {
        if (recoverySetState() == ParamSetImplicit) setRecoverySetState(ParamSetUndefined);
        else if (permeateSetState() == ParamSetImplicit) setPermeateSetState(ParamSetUndefined);
    }


    Q_EMIT feedSetStateChanged();
}
void ROPassParamSetController::setRecoverySetState(ParamSetState state) {
    if (_recoverySetState == state) return;
    _recoverySetState = state;
    if (state == ParamSetExplicit || state == ParamSetImplicit || state == ParamSetAuto) {
        if (permeateSetState() == ParamSetUndefined &&
                (feedSetState() == ParamSetExplicit ||
                 feedSetState() == ParamSetImplicit ||
                 feedSetState() == ParamSetAuto)) {
            setPermeateSetState(ParamSetImplicit);
        } else if (feedSetState() == ParamSetUndefined &&
                   (permeateSetState() == ParamSetExplicit ||
                    permeateSetState() == ParamSetImplicit ||
                    permeateSetState() == ParamSetAuto))  {
            setFeedSetState(ParamSetImplicit);
        }
    } else {
        if (feedSetState() == ParamSetImplicit) setFeedSetState(ParamSetUndefined);
        else if (permeateSetState() == ParamSetImplicit) setPermeateSetState(ParamSetUndefined);
    }

    Q_EMIT recoverySetStateChanged();
}
void ROPassParamSetController::setPermeateSetState(ParamSetState state) {
    if (_permeateSetState == state) return;
    _permeateSetState = state;
    if (state == ParamSetExplicit || state == ParamSetImplicit || state == ParamSetAuto) {
        if (recoverySetState() == ParamSetUndefined &&
                (feedSetState() == ParamSetExplicit ||
                 feedSetState() == ParamSetImplicit ||
                 feedSetState() == ParamSetAuto)) {
            setRecoverySetState(ParamSetImplicit);
        } else if (feedSetState() == ParamSetUndefined &&
                   (recoverySetState() == ParamSetExplicit ||
                    recoverySetState() == ParamSetImplicit ||
                    recoverySetState() == ParamSetAuto))  {
            setFeedSetState(ParamSetImplicit);
        }
    } else {
        if (feedSetState() == ParamSetImplicit) setFeedSetState(ParamSetUndefined);
        else if (recoverySetState() == ParamSetImplicit) setRecoverySetState(ParamSetUndefined);
    }

    Q_EMIT permeateSetStateChanged();
}


void ROPassParamSetController::reset() {
    if (feedSetState() == ParamSetExplicit) setFeedSetState(ParamSetUndefined);
    if (recoverySetState() == ParamSetExplicit) setRecoverySetState(ParamSetUndefined);
    if (permeateSetState() == ParamSetExplicit) setPermeateSetState(ParamSetUndefined);
}
