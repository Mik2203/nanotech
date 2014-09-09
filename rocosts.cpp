#include "rocosts.h"

// TODO CONSTS!

ROCosts::ROCosts(const ROSystem* const sys, QObject *parent) :
    _sys(sys),
    _systemCost(0.0),

    _microfiltersMembraneCount(0.8),
    _microfiltersYearChangeFrequency(4.0),
    _microfilterCost(260.0),

    _membranesYearChangeFrequency(0.33),
    _membraneCost(30000.0),

    _electricEnergyCost(2.0),

    _washingFrequency(2.0),

    _citricAcidCost(160.0),
    _H2SO4Cost(3.5),
    _trilonBCost(200.0),
    _alkaliCost(40.0),

    _antiscalantAmount(3.0),
    _antiscalantCost(200.0),
    _acidAntiscalantAmount(1.0),
    _acidAntiscalantCost(3.5),

    _rawWaterCost(0.33),
    _concentrateDropCost(0.1),

    _staffCount(0.5),
    _staffSalary(30000.0),
    _accruals(10000.0),
    _overheadRate(2.0),

    _systemLifetime(7),

    QObject(parent) {


    bindSystem();
    bindInternal();
}

ROCosts::ROCosts(): _sys(nullptr) { }

double ROCosts::permeateYearAmount() const { return _sys->permeate()->rate() * 24 * 365; }
double ROCosts::systemCost() const { return _systemCost; }

double ROCosts::systemSetCostYear() const { return microfiltersYearCost() + citricAcidYearCost() + H2SO4YearCost() + trilonBYearCost() +
            alkaliYearCost() + antiscalantYearCost() + acidAntiscalantYearCost(); }

double ROCosts::systemWithYearSetCost() const { return systemCost() + systemSetCostYear(); }

double ROCosts::microfiltersMembraneCount() const { return _microfiltersMembraneCount; }
double ROCosts::microfiltersSystemCount() const { return microfiltersMembraneCount() * _sys->elementsCount(); }
double ROCosts::microfiltersYearChangeFrequency() const { return _microfiltersYearChangeFrequency; }
double ROCosts::microfiltersYearCount() const { return microfiltersSystemCount() * microfiltersYearChangeFrequency(); }
double ROCosts::microfilterCost() const { return _microfilterCost; }
double ROCosts::microfiltersYearCost() const { return microfiltersYearCount() * microfilterCost(); }

//double ROCosts::membranesSystemCount() const { return _sys->elementsCount(); }
double ROCosts::membranesYearChangeFrequency() const { return _membranesYearChangeFrequency; }
double ROCosts::membranesYearCount() const { return _sys->elementsCount() * membranesYearChangeFrequency(); }
double ROCosts::membraneCost() const { return _membraneCost; }
double ROCosts::membranesYearCost() const { return membraneCost() * membranesYearCount(); }

double ROCosts::electricEnergyYearAmount() const { return _sys->power() * 24 * 365; }
double ROCosts::electricEnergyCost() const { return _electricEnergyCost; }
double ROCosts::electricEnergyYearCost() const { return electricEnergyYearAmount() * electricEnergyCost(); }

double ROCosts::totalCostsYearSystem() const { return microfiltersYearCost() + membranesYearCost() + electricEnergyYearCost(); }
double ROCosts::totalWaterCostsYearSystem() const { return permeateYearAmount() > 0.0 ? totalCostsYearSystem() / permeateYearAmount() : 0.0; }
double ROCosts::washingSolutionVolume() const { return _sys->elementsCount() * 0.045; }

double ROCosts::washingFrequency() const { return _washingFrequency; }



void ROCosts::reset() {
    // TODO
}

void ROCosts::copyDataFrom(const ROCosts *const from) {
    // TODO
}


void ROCosts::setMicrofiltersMembraneCount(double value) {
    // TODO MAX MIN
    _microfiltersMembraneCount = value;
    Q_EMIT microfiltersMembraneCountChanged();
}

void ROCosts::setMicrofiltersYearChangeFrequency(double value) {
    // TODO MAX MIN
    _microfiltersYearChangeFrequency = value;
    Q_EMIT microfiltersYearChangeFrequencyChanged();
}

void ROCosts::setMicrofilterCost(double value) {
    // TODO MAX MIN
    _microfilterCost = value;
    Q_EMIT microfilterCostChanged();
}

void ROCosts::setMembranesYearChangeFrequency(double value) {
    // TODO MAX MIN
    _membranesYearChangeFrequency = value;
    Q_EMIT membranesYearChangeFrequencyChanged();
}

void ROCosts::setMembraneCost(double value) {
    // TODO MAX MIN
    _membraneCost = value;
    Q_EMIT membraneCostChanged();
}

void ROCosts::setElectricEnergyCost(double value) {
    // TODO MAX MIN
    _electricEnergyCost = value;
    Q_EMIT electricEnergyCostChanged();
}

void ROCosts::setWashingFrequency(double value) {
    // TODO MAX MIN
    _washingFrequency = value;
    Q_EMIT washingFrequencyChanged();
}

void ROCosts::setCitricAcidCost(double value) {
    // TODO MAX MIN
    _citricAcidCost = value;
    Q_EMIT washingFrequencyChanged();
}

void ROCosts::setH2SO4Cost(double value) {
    // TODO MAX MIN
    _H2SO4Cost = value;
    Q_EMIT H2SO4CostChanged();
}

void ROCosts::setTrilonBCost(double value) {
    // TODO MAX MIN
    _trilonBCost = value;
    Q_EMIT trilonBCostChanged();
}


void ROCosts::setAlkaliCost(double value) {
    // TODO MAX MIN
    _alkaliCost = value;
    Q_EMIT alkaliCostChanged();
}

void ROCosts::setAntiscalantAmount(double value) {
    // TODO MAX MIN
    _antiscalantAmount = value;
    Q_EMIT antiscalantAmountChanged();
}

void ROCosts::setAntiscalantCost(double value) {
    // TODO MAX MIN
    _antiscalantCost = value;
    Q_EMIT antiscalantCostChanged();
}

void ROCosts::setAcidAntiscalantAmount(double value) {
    // TODO MAX MIN
    _acidAntiscalantAmount = value;
    Q_EMIT acidAntiscalantAmountChanged();
}

void ROCosts::setAcidAntiscalantCost(double value) {
    // TODO MAX MIN
    _acidAntiscalantCost = value;
    Q_EMIT acidAntiscalantCostChanged();
}

void ROCosts::setRawWaterCost(double value) {
    // TODO MAX MIN
    _rawWaterCost = value;
    Q_EMIT rawWaterCostChanged();
}

void ROCosts::setConcentrateDropCost(double value) {
    // TODO MAX MIN
    _concentrateDropCost = value;
    Q_EMIT concentrateDropCostChanged();
}

void ROCosts::setStaffCount(double value) {
    // TODO MAX MIN
    _staffCount = value;
    Q_EMIT staffCountChanged();
}

void ROCosts::setStaffSalary(double value) {
    // TODO MAX MIN
    _staffSalary = value;
    Q_EMIT staffSalaryChanged();
}

void ROCosts::setOverheadRate(double value) {
    // TODO MAX MIN
    _overheadRate = value;
    Q_EMIT overheadRateChanged();
}

void ROCosts::setAccruals(double value) {
    // TODO MAX MIN
    _accruals = value;
    Q_EMIT accrualsChanged();
}

void ROCosts::setSystemLifetime(double value) {
    // TODO MAX MIN
    _systemLifetime = value;
    Q_EMIT systemLifetimeChanged();
}

void ROCosts::setSystemCost(double value) {
    // TODO MAX MIN
    disconnect(_sys->permeate(), SIGNAL(rateChanged()), this, SLOT(updateSystemCost()));
    _systemCost = value;
    Q_EMIT systemCostChanged();
}

void ROCosts::updateSystemCost() {
    _systemCost = _sys->permeate()->rate() * 90000.0;
    Q_EMIT systemCostChanged();
}

void ROCosts::bindSystem()
{
    connect(_sys->feed(), SIGNAL(rateChanged()), this, SIGNAL(rawWaterYearAmountChanged()));
    connect(_sys->concentrate(), SIGNAL(rateChanged()), this, SIGNAL(concentrateDropYearAmountChanged()));
    connect(_sys->permeate(), SIGNAL(rateChanged()), this, SIGNAL(permeateYearAmountChanged()));
    connect(_sys->permeate(), SIGNAL(rateChanged()), this, SLOT(updateSystemCost()));
    connect(_sys->permeate(), SIGNAL(rateChanged()), this, SIGNAL(antiscalantSystemAmountChanged()));
    connect(_sys->permeate(), SIGNAL(rateChanged()), this, SIGNAL(acidAntiscalantSystemAmountChanged()));
    connect(_sys, SIGNAL(elementsCountChanged()), this, SIGNAL(microfiltersSystemCountChanged()));
    connect(_sys, SIGNAL(elementsCountChanged()), this, SIGNAL(membranesYearCountChanged()));
    connect(_sys, SIGNAL(elementsCountChanged()), this, SIGNAL(washingSolutionVolumeChanged()));
    connect(_sys, SIGNAL(powerChanged()), this, SIGNAL(electricEnergyYearAmountChanged()));
}

void ROCosts::bindPermeateYearAmount()
{
    connect(this, SIGNAL(permeateYearAmountChanged()), this, SIGNAL(totalWaterCostsYearSystemChanged()));
    connect(this, SIGNAL(permeateYearAmountChanged()), this, SIGNAL(totalWaterCostsYearScalingChanged()));
    connect(this, SIGNAL(permeateYearAmountChanged()), this, SIGNAL(totalWaterCostsYearWaterChanged()));
    connect(this, SIGNAL(permeateYearAmountChanged()), this, SIGNAL(totalWaterCostsYearMaintenanceChanged()));
    connect(this, SIGNAL(permeateYearAmountChanged()), this, SIGNAL(totalWaterCostsYearChanged()));
}

void ROCosts::bindSystemCost()
{
    connect(this, SIGNAL(systemCostChanged()), this, SIGNAL(systemWithYearSetCostChanged()));
    connect(this, SIGNAL(systemCostSetYearChanged()), this, SIGNAL(systemWithYearSetCostChanged()));
}

void ROCosts::bindSystemSection()
{
    bindMicrofilters();
    bindMembranes();
    bindEnergy();

    connect(this, SIGNAL(totalCostsYearSystemChanged()), this, SIGNAL(totalWaterCostsYearSystemChanged()));
    connect(this, SIGNAL(totalCostsYearSystemChanged()), this, SIGNAL(totalCostsYearChanged()));
}

void ROCosts::bindMicrofilters()
{
    connect(this, SIGNAL(microfiltersMembraneCountChanged()), this, SIGNAL(microfiltersSystemCountChanged()));

    connect(this, SIGNAL(microfiltersSystemCountChanged()), this, SIGNAL(microfiltersYearCountChanged()));
    connect(this, SIGNAL(microfiltersYearChangeFrequencyChanged()), this, SIGNAL(microfiltersYearCountChanged()));

    connect(this, SIGNAL(microfiltersYearCountChanged()), this, SIGNAL(microfiltersYearCostChanged()));
    connect(this, SIGNAL(microfilterCostChanged()), this, SIGNAL(microfiltersYearCostChanged()));

    connect(this, SIGNAL(microfiltersYearCostChanged()), this, SIGNAL(totalCostsYearSystemChanged()));
    connect(this, SIGNAL(microfiltersYearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindMembranes()
{
    connect(this, SIGNAL(membranesYearChangeFrequencyChanged()), this, SIGNAL(membranesYearCountChanged()));

    connect(this, SIGNAL(membranesYearCountChanged()), this, SIGNAL(membranesYearCostChanged()));
    connect(this, SIGNAL(membraneCostChanged()), this, SIGNAL(membranesYearCostChanged()));

    connect(this, SIGNAL(membranesYearCostChanged()), this, SIGNAL(totalCostsYearSystemChanged()));
}

void ROCosts::bindEnergy()
{
    connect(this, SIGNAL(electricEnergyYearAmountChanged()), this, SIGNAL(electricEnergyYearCostChanged()));
    connect(this, SIGNAL(electricEnergyCostChanged()), this, SIGNAL(electricEnergyYearCostChanged()));

    connect(this, SIGNAL(electricEnergyYearCostChanged()), this, SIGNAL(totalCostsYearSystemChanged()));
}

void ROCosts::bindScalingSection()
{
    bindCitricAcid();
    bindH2SO4();
    bindTrilonB();
    bindAlkali();
    bindAntiscalant();
    bindAcidAntiscalant();

    connect(this, SIGNAL(totalCostsYearScalingChanged()), this, SIGNAL(totalWaterCostsYearScalingChanged()));
    connect(this, SIGNAL(totalCostsYearScalingChanged()), this, SIGNAL(totalCostsYearChanged()));
}

void ROCosts::bindCitricAcid()
{
    connect(this, SIGNAL(washingSolutionVolumeChanged()), this, SIGNAL(citricAcidYearAmountChanged()));
    connect(this, SIGNAL(washingFrequencyChanged()), this, SIGNAL(citricAcidYearAmountChanged()));

    connect(this, SIGNAL(citricAcidYearAmountChanged()), this, SIGNAL(citricAcidYearCostChanged()));
    connect(this, SIGNAL(citricAcidCostChanged()), this, SIGNAL(citricAcidYearCostChanged()));

    connect(this, SIGNAL(citricAcidYearCostChanged()), this, SIGNAL(totalCostsYearScalingChanged()));
    connect(this, SIGNAL(citricAcidYearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindH2SO4()
{
    connect(this, SIGNAL(washingSolutionVolumeChanged()), this, SIGNAL(H2SO4YearAmountChanged()));
    connect(this, SIGNAL(washingFrequencyChanged()), this, SIGNAL(H2SO4YearAmountChanged()));

    connect(this, SIGNAL(H2SO4YearAmountChanged()), this, SIGNAL(H2SO4YearCostChanged()));
    connect(this, SIGNAL(H2SO4CostChanged()), this, SIGNAL(H2SO4YearCostChanged()));

    connect(this, SIGNAL(H2SO4YearCostChanged()), this, SIGNAL(totalCostsYearScalingChanged()));
    connect(this, SIGNAL(H2SO4YearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindTrilonB()
{
    connect(this, SIGNAL(washingSolutionVolumeChanged()), this, SIGNAL(trilonBYearAmountChanged()));
    connect(this, SIGNAL(washingFrequencyChanged()), this, SIGNAL(trilonBYearAmountChanged()));

    connect(this, SIGNAL(trilonBYearAmountChanged()), this, SIGNAL(trilonBYearCostChanged()));
    connect(this, SIGNAL(trilonBCostChanged()), this, SIGNAL(trilonBYearCostChanged()));

    connect(this, SIGNAL(trilonBYearCostChanged()), this, SIGNAL(totalCostsYearScalingChanged()));
    connect(this, SIGNAL(trilonBYearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindAlkali()
{
    connect(this, SIGNAL(washingSolutionVolumeChanged()), this, SIGNAL(alkaliYearAmountChanged()));
    connect(this, SIGNAL(washingFrequencyChanged()), this, SIGNAL(alkaliYearAmountChanged()));

    connect(this, SIGNAL(alkaliYearAmountChanged()), this, SIGNAL(alkaliYearCostChanged()));
    connect(this, SIGNAL(alkaliCostChanged()), this, SIGNAL(alkaliYearCostChanged()));

    connect(this, SIGNAL(alkaliYearCostChanged()), this, SIGNAL(totalCostsYearScalingChanged()));
    connect(this, SIGNAL(alkaliYearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindAntiscalant()
{
    connect(this, SIGNAL(antiscalantAmountChanged()), this, SIGNAL(antiscalantSystemAmountChanged()));
    connect(this, SIGNAL(antiscalantSystemAmountChanged()), this, SIGNAL(antiscalantYearCostChanged()));
    connect(this, SIGNAL(antiscalantCostChanged()), this, SIGNAL(antiscalantYearCostChanged()));

    connect(this, SIGNAL(antiscalantYearCostChanged()), this, SIGNAL(totalCostsYearScalingChanged()));
    connect(this, SIGNAL(antiscalantYearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindAcidAntiscalant()
{

    connect(this, SIGNAL(acidAntiscalantAmountChanged()), this, SIGNAL(acidAntiscalantSystemAmountChanged()));
    connect(this, SIGNAL(acidAntiscalantSystemAmountChanged()), this, SIGNAL(acidAntiscalantYearCostChanged()));
    connect(this, SIGNAL(acidAntiscalantCostChanged()), this, SIGNAL(acidAntiscalantYearCostChanged()));

    connect(this, SIGNAL(acidAntiscalantYearCostChanged()), this, SIGNAL(totalCostsYearScalingChanged()));
    connect(this, SIGNAL(acidAntiscalantYearCostChanged()), this, SIGNAL(systemCostSetYearChanged()));
}

void ROCosts::bindWaterSection()
{
    bindRawWater();
    bindConcentrateDrop();

    connect(this, SIGNAL(totalCostsYearWaterChanged()), this, SIGNAL(totalWaterCostsYearWaterChanged()));
    connect(this, SIGNAL(totalCostsYearWaterChanged()), this, SIGNAL(totalCostsYearChanged()));
}

void ROCosts::bindRawWater()
{
    connect(this, SIGNAL(rawWaterCostChanged()), this, SIGNAL(rawWaterYearCostChanged()));
    connect(this, SIGNAL(rawWaterYearAmountChanged()), this, SIGNAL(rawWaterYearCostChanged()));

    connect(this, SIGNAL(rawWaterYearCostChanged()), this, SIGNAL(totalCostsYearWaterChanged()));
}

void ROCosts::bindConcentrateDrop()
{
    connect(this, SIGNAL(concentrateDropCostChanged()), this, SIGNAL(concentrateDropYearCostChanged()));
    connect(this, SIGNAL(concentrateDropYearAmountChanged()), this, SIGNAL(concentrateDropYearCostChanged()));

    connect(this, SIGNAL(concentrateDropYearCostChanged()), this, SIGNAL(totalCostsYearWaterChanged()));
}

void ROCosts::bindMaintainanceSection()
{
    bindStaff();
    bindAccruals();
    bindDeductions();

    connect(this, SIGNAL(totalCostsYearMaintenanceChanged()), this, SIGNAL(totalWaterCostsYearMaintenanceChanged()));
    connect(this, SIGNAL(totalCostsYearMaintenanceChanged()), this, SIGNAL(totalCostsYearChanged()));
}

void ROCosts::bindStaff()
{
    connect(this, SIGNAL(staffCountChanged()), this, SIGNAL(staffYearSalaryChanged()));
    connect(this, SIGNAL(staffSalaryChanged()), this, SIGNAL(staffYearSalaryChanged()));

    connect(this, SIGNAL(staffYearSalaryChanged()), this, SIGNAL(staffYearCostChanged()));
    connect(this, SIGNAL(overheadRateChanged()), this, SIGNAL(staffYearCostChanged()));

    connect(this, SIGNAL(staffYearCostChanged()), this, SIGNAL(totalCostsYearMaintenanceChanged()));
}

void ROCosts::bindAccruals()
{
    connect(this, SIGNAL(staffCountChanged()), this, SIGNAL(accrualsYearChanged()));
    connect(this, SIGNAL(accrualsChanged()), this, SIGNAL(accrualsYearChanged()));

    connect(this, SIGNAL(accrualsYearChanged()), this, SIGNAL(accrualsYearCostChanged()));
    connect(this, SIGNAL(overheadRateChanged()), this, SIGNAL(accrualsYearCostChanged()));

    connect(this, SIGNAL(accrualsYearCostChanged()), this, SIGNAL(totalCostsYearMaintenanceChanged()));
}

void ROCosts::bindDeductions()
{
    connect(this, SIGNAL(systemCostChanged()), this, SIGNAL(deductionsYearCostChanged()));
    connect(this, SIGNAL(systemLifetimeChanged()), this, SIGNAL(deductionsYearCostChanged()));
    connect(this, SIGNAL(deductionsYearCostChanged()), this, SIGNAL(totalCostsYearMaintenanceChanged()));
}

void ROCosts::bindInternal()
{
    bindPermeateYearAmount();
    bindSystemCost();
    bindSystemSection();
    bindScalingSection();
    bindWaterSection();
    bindMaintainanceSection();
    connect(this, SIGNAL(totalCostsYearChanged()), this, SIGNAL(totalWaterCostsYearChanged()));
}


double ROCosts::citricAcidAmount() const { return washingSolutionVolume() * 20.3166; }
double ROCosts::citricAcidYearAmount() const { return citricAcidAmount() * washingFrequency(); }
double ROCosts::citricAcidCost() const { return _citricAcidCost; }
double ROCosts::citricAcidYearCost() const { return citricAcidYearAmount() * citricAcidCost(); }

double ROCosts::H2SO4Amount() const { return washingSolutionVolume() * 0.4745; }
double ROCosts::H2SO4YearAmount() const { return H2SO4Amount() * washingFrequency(); }
double ROCosts::H2SO4Cost() const { return _H2SO4Cost; }
double ROCosts::H2SO4YearCost() const { return H2SO4YearAmount() * H2SO4Cost(); }

double ROCosts::trilonBAmount() const { return washingSolutionVolume() * 8.3905; }
double ROCosts::trilonBYearAmount() const { return trilonBAmount() * washingFrequency(); }
double ROCosts::trilonBCost() const { return _trilonBCost; }
double ROCosts::trilonBYearCost() const { return trilonBYearAmount() * trilonBCost(); }

double ROCosts::alkaliAmount() const { return washingSolutionVolume() * 0.399996; }
double ROCosts::alkaliYearAmount() const { return alkaliAmount() * washingFrequency(); }
double ROCosts::alkaliCost() const { return _alkaliCost; }
double ROCosts::alkaliYearCost() const { return alkaliYearAmount() * alkaliCost(); }

double ROCosts::antiscalantAmount() const { return _antiscalantAmount; }
double ROCosts::antiscalantSystemAmount() const { return antiscalantAmount() * _sys->permeate()->rate() * 0.001; }
double ROCosts::antiscalantYearAmount() const { return antiscalantSystemAmount() * 24 * 365; }
double ROCosts::antiscalantCost() const { return _antiscalantCost; }
double ROCosts::antiscalantYearCost() const { return antiscalantYearAmount() * antiscalantCost(); }

double ROCosts::acidAntiscalantAmount() const { return _acidAntiscalantAmount; }
double ROCosts::acidAntiscalantSystemAmount() const { return acidAntiscalantAmount() * _sys->permeate()->rate() * 0.001; }
double ROCosts::acidAntiscalantYearAmount() const { return acidAntiscalantSystemAmount() * 24 * 365; }
double ROCosts::acidAntiscalantCost() const { return _acidAntiscalantCost; }
double ROCosts::acidAntiscalantYearCost() const { return acidAntiscalantYearAmount() * acidAntiscalantCost(); }

double ROCosts::totalCostsYearScaling() const { return citricAcidYearCost() + H2SO4YearCost() + trilonBYearCost() +
            alkaliYearCost() + antiscalantYearCost() + acidAntiscalantYearCost(); }
double ROCosts::totalWaterCostsYearScaling() const { return permeateYearAmount() > 0.0 ? totalCostsYearScaling() / permeateYearAmount() : 0.0; }

double ROCosts::rawWaterYearAmount() const { return _sys->feed()->rate() * 24 * 365; }
double ROCosts::rawWaterCost() const { return _rawWaterCost; }
double ROCosts::rawWaterYearCost() const { return rawWaterYearAmount() * rawWaterCost(); }

double ROCosts::concentrateDropYearAmount() const { return _sys->concentrate()->rate() * 24 * 365; }
double ROCosts::concentrateDropCost() const { return _concentrateDropCost; }
double ROCosts::concentrateDropYearCost() const { return concentrateDropYearAmount() * concentrateDropCost(); }

double ROCosts::totalCostsYearWater() const { return rawWaterYearCost() + concentrateDropYearCost(); }
double ROCosts::totalWaterCostsYearWater() const { return permeateYearAmount() > 0.0 ? totalCostsYearWater() / permeateYearAmount() : 0.0; }


double ROCosts::staffCount() const { return _staffCount; }
double ROCosts::staffSalary() const { return _staffSalary; }
double ROCosts::staffYearSalary() const { return staffSalary() * staffCount() * 12; }
double ROCosts::overheadRate() const { return _overheadRate; }
double ROCosts::staffYearCost() const { return staffYearSalary() * overheadRate(); }

double ROCosts::accruals() const { return _accruals; }
double ROCosts::accrualsYear() const { return accruals() * staffCount() * 12; }
double ROCosts::accrualsYearCost() const { return accrualsYear() * overheadRate(); }

double ROCosts::systemLifetime() const { return _systemLifetime; }
double ROCosts::deductionsYearCost() const { return systemLifetime() > 0.0 ? systemCost() / systemLifetime() : 0.0; }

double ROCosts::totalCostsYearMaintenance() const { return staffYearCost() + accrualsYearCost() + deductionsYearCost(); }
double ROCosts::totalWaterCostsYearMaintenance() const { return permeateYearAmount() > 0.0 ? totalCostsYearMaintenance() / permeateYearAmount() : 0.0; }

double ROCosts::totalCostsYear() const { return totalCostsYearSystem() + totalCostsYearScaling() + totalCostsYearWater() + totalCostsYearMaintenance(); }
double ROCosts::totalWaterCostsYear() const { return permeateYearAmount() > 0.0 ? totalCostsYear() / permeateYearAmount() : 0.0; }

