#include "rocase.h"

ROCase::ROCase() :
    _sys(new ROSystem()) {
    _costs = new ROCosts(_sys);
    _sysSS = new ROSystemSolveScheduler(_sys);
    _sysC = new ROSystemController(_sys, _sysSS);

    connect(_sysC, SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
    connect(_sysC, SIGNAL(inputChanged()), _sysSS, SLOT(resetCalculated()));
}

ROCase::~ROCase() {
    delete _sys;
    delete _sysC;
    delete _sysSS;
    delete _costs;
}

void ROCase::copyDataFrom(const ROCase* const from) {
    this->sys()->copyDataFrom(from->sys());
    this->sysC()->copyDataFrom(from->sysC());
    this->costs()->copyDataFrom(from->costs());
}


ROSystem* const ROCase::sys() const { return _sys; }
ROCosts *const ROCase::costs() const { return _costs; }
ROSystemController* const ROCase::sysC() const { return _sysC; }

ROSystemSolveScheduler *const ROCase::sysSS() const { return _sysSS; }


void ROCase::reset() {
    sys()->reset();
    sysC()->reset();
    costs()->reset();
}
