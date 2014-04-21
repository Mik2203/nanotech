#include "ropassparamsetcontroller.h"

ROPassParamSetController::ROPassParamSetController(QObject *parent) :
    //SET STATES
    _feedSetState(ParamSetUndefined),
    _recoverySetState(ParamSetUndefined),
    _permeateSetState(ParamSetUndefined),
    _statesChanging(false),
    _next(0),
    _prev(0),
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

void ROPassParamSetController::setNextController(ROPassParamSetController* const next) {
    if (next == _next) return;
    _next = next;
    if (!next) {
        if (this->permeateSetState() == ParamSetAuto) {
            this->setPermeateSetState(ParamSetUndefined);
            return;
        }
    } else {
        next->setPrevController(this);
    }
}

void ROPassParamSetController::setPrevController(ROPassParamSetController* const prev) {
    if (prev == _prev) return;
    _prev = prev;
    if (!prev){
        if (this->feedSetState() == ParamSetAuto) {
            this->setFeedSetState(ParamSetUndefined);
            return;
        }
    } else {

        switch (prev->permeateSetState()) {
        case ParamSetUndefined:
            switch(this->feedSetState()) {
            case ParamSetExplicit: prev->setPermeateSetState(ParamSetExplicit); break;
            case ParamSetImplicit: prev->setPermeateSetState(ParamSetAuto); break;
            case ParamSetAuto: this->setFeedSetState(ParamSetUndefined); break;
            }
            break;
        case ParamSetExplicit:
            switch(this->feedSetState()) {
            case ParamSetUndefined: this->setFeedSetState(ParamSetExplicit); break;
            case ParamSetImplicit: prev->setPermeateSetState(ParamSetAuto); break;
            case ParamSetAuto: this->setFeedSetState(ParamSetExplicit); break;
            }
            break;
        case ParamSetImplicit:
            switch(this->feedSetState()) {
            case ParamSetUndefined:
            case ParamSetExplicit: this->setFeedSetState(ParamSetAuto); break;
            case ParamSetImplicit:
                this->setRecoverySetState(ParamSetUndefined);
                this->setFeedSetState(ParamSetAuto);
                break;
            case ParamSetAuto: break;
            }

            break;
        case ParamSetAuto:
            switch(this->feedSetState()) {
            case ParamSetUndefined: prev->setPermeateSetState(ParamSetUndefined); break;
            case ParamSetExplicit: prev->setPermeateSetState(ParamSetExplicit); break;
            case ParamSetImplicit: break;
            case ParamSetAuto:
                prev->setPermeateSetState(ParamSetUndefined);
                this->setFeedSetState(ParamSetUndefined);
                break;
            }

        }
        prev->_next = this;
    }
}

bool ROPassParamSetController::allSet() const {
    return ((feedSetState() == ParamSetExplicit || feedSetState() == ParamSetImplicit || feedSetState() == ParamSetAuto) &&
            (recoverySetState() == ParamSetExplicit || recoverySetState() == ParamSetImplicit || recoverySetState() == ParamSetAuto) &&
            (permeateSetState() == ParamSetExplicit || permeateSetState() == ParamSetImplicit || permeateSetState() == ParamSetAuto));

}

void ROPassParamSetController::setFeedSetState(ParamSetState state) { // TODO CHECK!

    if (_feedSetState == state) return;
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
        if (_prev) {
            if (state == ParamSetExplicit) _prev->setPermeateSetState(ParamSetExplicit);
            if (state == ParamSetImplicit) _prev->setPermeateSetState(ParamSetAuto);
        }
    } else {
        if (recoverySetState() == ParamSetImplicit) setRecoverySetState(ParamSetUndefined);
        else if (permeateSetState() == ParamSetImplicit) setPermeateSetState(ParamSetUndefined);

        if (_prev)  _prev->setPermeateSetState(ParamSetUndefined);
    }


    emit feedSetStateChanged();
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

    emit recoverySetStateChanged();
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
        if (_next) {
            if (state == ParamSetExplicit) _next->setFeedSetState(ParamSetExplicit);
            if (state == ParamSetImplicit) _next->setFeedSetState(ParamSetAuto);
        }
    } else {
            if (feedSetState() == ParamSetImplicit) setFeedSetState(ParamSetUndefined);
            else if (recoverySetState() == ParamSetImplicit) setRecoverySetState(ParamSetUndefined);
        if (_next) _next->setFeedSetState(ParamSetUndefined);
    }

    emit permeateSetStateChanged();
}


void ROPassParamSetController::reset() {
    if (feedSetState() == ParamSetExplicit) setFeedSetState(ParamSetUndefined);
    if (recoverySetState() == ParamSetExplicit) setRecoverySetState(ParamSetUndefined);
    if (permeateSetState() == ParamSetExplicit) setPermeateSetState(ParamSetUndefined);
}
