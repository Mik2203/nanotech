#ifndef ROCOSTS_H
#define ROCOSTS_H

#include <QObject>
#include "rosystem.h"



// содержит данные и методы расчета затрат на обслуживание системы
class ROCosts : public QObject
{
    Q_OBJECT

    // SYSTEM
    Q_PROPERTY(double permeateYearAmount READ permeateYearAmount NOTIFY permeateYearAmountChanged)
    Q_PROPERTY(double systemCost READ systemCost WRITE setSystemCost NOTIFY systemCostChanged)
    Q_PROPERTY(double systemSetCostYear READ systemSetCostYear NOTIFY systemCostSetYearChanged)
    Q_PROPERTY(double systemWithYearSetCost READ systemWithYearSetCost NOTIFY systemWithYearSetCostChanged)

    Q_PROPERTY(double microfiltersMembraneCount READ microfiltersMembraneCount WRITE setMicrofiltersMembraneCount NOTIFY microfiltersMembraneCountChanged)
    Q_PROPERTY(double microfiltersSystemCount READ microfiltersSystemCount NOTIFY microfiltersSystemCountChanged)
    Q_PROPERTY(double microfiltersYearChangeFrequency READ microfiltersYearChangeFrequency WRITE setMicrofiltersYearChangeFrequency NOTIFY microfiltersYearChangeFrequencyChanged)
    Q_PROPERTY(double microfiltersYearCount READ microfiltersYearCount NOTIFY microfiltersYearCountChanged)
    Q_PROPERTY(double microfilterCost READ microfilterCost WRITE setMicrofilterCost NOTIFY microfilterCostChanged)
    Q_PROPERTY(double microfiltersYearCost READ microfiltersYearCost NOTIFY microfiltersYearCostChanged)

    Q_PROPERTY(double membranesYearChangeFrequency READ membranesYearChangeFrequency WRITE setMembranesYearChangeFrequency NOTIFY membranesYearChangeFrequencyChanged)
    Q_PROPERTY(double membranesYearCount READ membranesYearCount NOTIFY membranesYearCountChanged)
    Q_PROPERTY(double membraneCost READ membraneCost WRITE setMembraneCost NOTIFY membraneCostChanged)
    Q_PROPERTY(double membranesYearCost READ membranesYearCost NOTIFY membranesYearCostChanged)

    Q_PROPERTY(double electricEnergyYearAmount READ electricEnergyYearAmount NOTIFY electricEnergyYearAmountChanged)
    Q_PROPERTY(double electricEnergyCost READ electricEnergyCost WRITE setElectricEnergyCost NOTIFY electricEnergyCostChanged)
    Q_PROPERTY(double electricEnergyYearCost READ electricEnergyYearCost NOTIFY electricEnergyYearCostChanged)

    Q_PROPERTY(double totalCostsYearSystem READ totalCostsYearSystem NOTIFY totalCostsYearSystemChanged)
    Q_PROPERTY(double totalWaterCostsYearSystem READ totalWaterCostsYearSystem NOTIFY totalWaterCostsYearSystemChanged)

    // SCALING
    Q_PROPERTY(double washingSolutionVolume READ washingSolutionVolume NOTIFY washingSolutionVolumeChanged)
    Q_PROPERTY(double washingFrequency READ washingFrequency WRITE setWashingFrequency NOTIFY washingFrequencyChanged)

    Q_PROPERTY(double citricAcidAmount READ citricAcidAmount NOTIFY washingSolutionVolumeChanged)
    Q_PROPERTY(double citricAcidYearAmount READ citricAcidYearAmount NOTIFY citricAcidYearAmountChanged)
    Q_PROPERTY(double citricAcidCost READ citricAcidCost WRITE setCitricAcidCost NOTIFY citricAcidCostChanged)
    Q_PROPERTY(double citricAcidYearCost READ citricAcidYearCost NOTIFY citricAcidYearCostChanged)


    Q_PROPERTY(double H2SO4Amount READ H2SO4Amount NOTIFY washingSolutionVolumeChanged)
    Q_PROPERTY(double H2SO4YearAmount READ H2SO4YearAmount NOTIFY H2SO4YearAmountChanged)
    Q_PROPERTY(double H2SO4Cost READ H2SO4Cost WRITE setH2SO4Cost NOTIFY H2SO4CostChanged)
    Q_PROPERTY(double H2SO4YearCost READ H2SO4YearCost NOTIFY H2SO4YearCostChanged)


    Q_PROPERTY(double trilonBAmount READ trilonBAmount NOTIFY washingSolutionVolumeChanged)
    Q_PROPERTY(double trilonBYearAmount READ trilonBYearAmount NOTIFY trilonBYearAmountChanged)
    Q_PROPERTY(double trilonBCost READ trilonBCost WRITE setTrilonBCost NOTIFY trilonBCostChanged)
    Q_PROPERTY(double trilonBYearCost READ trilonBYearCost NOTIFY trilonBYearCostChanged)

    Q_PROPERTY(double alkaliAmount READ alkaliAmount NOTIFY washingSolutionVolumeChanged)
    Q_PROPERTY(double alkaliYearAmount READ alkaliYearAmount NOTIFY alkaliYearAmountChanged)
    Q_PROPERTY(double alkaliCost READ alkaliCost WRITE setAlkaliCost NOTIFY alkaliCostChanged)
    Q_PROPERTY(double alkaliYearCost READ alkaliYearCost NOTIFY alkaliYearCostChanged)

    Q_PROPERTY(double antiscalantAmount READ antiscalantAmount WRITE setAntiscalantAmount NOTIFY antiscalantAmountChanged)
    Q_PROPERTY(double antiscalantSystemAmount READ antiscalantSystemAmount NOTIFY antiscalantSystemAmountChanged)
    Q_PROPERTY(double antiscalantYearAmount READ antiscalantYearAmount NOTIFY antiscalantSystemAmountChanged)
    Q_PROPERTY(double antiscalantCost READ antiscalantCost WRITE setAntiscalantCost NOTIFY antiscalantCostChanged)
    Q_PROPERTY(double antiscalantYearCost READ antiscalantYearCost NOTIFY antiscalantYearCostChanged)

    Q_PROPERTY(double acidAntiscalantAmount READ acidAntiscalantAmount WRITE setAcidAntiscalantAmount NOTIFY acidAntiscalantAmountChanged)
    Q_PROPERTY(double acidAntiscalantSystemAmount READ acidAntiscalantSystemAmount NOTIFY acidAntiscalantSystemAmountChanged)
    Q_PROPERTY(double acidAntiscalantYearAmount READ acidAntiscalantYearAmount NOTIFY acidAntiscalantSystemAmountChanged)
    Q_PROPERTY(double acidAntiscalantCost READ acidAntiscalantCost WRITE setAcidAntiscalantCost NOTIFY acidAntiscalantCostChanged)
    Q_PROPERTY(double acidAntiscalantYearCost READ acidAntiscalantYearCost NOTIFY acidAntiscalantYearCostChanged)

    Q_PROPERTY(double totalCostsYearScaling READ totalCostsYearScaling NOTIFY totalCostsYearScalingChanged)
    Q_PROPERTY(double totalWaterCostsYearScaling READ totalWaterCostsYearScaling NOTIFY totalWaterCostsYearScalingChanged)

    // WATER
    Q_PROPERTY(double rawWaterYearAmount READ rawWaterYearAmount NOTIFY rawWaterYearAmountChanged)
    Q_PROPERTY(double rawWaterCost READ rawWaterCost WRITE setRawWaterCost NOTIFY rawWaterCostChanged)
    Q_PROPERTY(double rawWaterYearCost READ rawWaterYearCost NOTIFY rawWaterYearCostChanged)

    Q_PROPERTY(double concentrateDropYearAmount READ concentrateDropYearAmount NOTIFY concentrateDropYearAmountChanged)
    Q_PROPERTY(double concentrateDropCost READ concentrateDropCost WRITE setConcentrateDropCost NOTIFY concentrateDropCostChanged)
    Q_PROPERTY(double concentrateDropYearCost READ concentrateDropYearCost NOTIFY concentrateDropYearCostChanged)

    Q_PROPERTY(double totalCostsYearWater READ totalCostsYearWater NOTIFY totalCostsYearWaterChanged)
    Q_PROPERTY(double totalWaterCostsYearWater READ totalWaterCostsYearWater NOTIFY totalWaterCostsYearWaterChanged)


    // maintenance
    Q_PROPERTY(double staffCount READ staffCount WRITE setStaffCount NOTIFY staffCountChanged)
    Q_PROPERTY(double staffSalary READ staffSalary WRITE setStaffSalary NOTIFY staffSalaryChanged)
    Q_PROPERTY(double staffYearSalary READ staffYearSalary NOTIFY staffYearSalaryChanged)
    Q_PROPERTY(double overheadRate READ overheadRate WRITE setOverheadRate NOTIFY overheadRateChanged)
    Q_PROPERTY(double staffYearCost READ staffYearCost NOTIFY staffYearCostChanged)

    Q_PROPERTY(double accruals READ accruals WRITE setAccruals NOTIFY accrualsChanged)
    Q_PROPERTY(double accrualsYear READ accrualsYear NOTIFY accrualsYearChanged)
    Q_PROPERTY(double accrualsYearCost READ accrualsYearCost NOTIFY accrualsYearCostChanged)

    Q_PROPERTY(double systemLifetime READ systemLifetime WRITE setSystemLifetime NOTIFY systemLifetimeChanged)
    Q_PROPERTY(double deductionsYearCost READ deductionsYearCost NOTIFY deductionsYearCostChanged)


    Q_PROPERTY(double totalCostsYearMaintenance READ totalCostsYearMaintenance NOTIFY totalCostsYearMaintenanceChanged)
    Q_PROPERTY(double totalWaterCostsYearMaintenance READ totalWaterCostsYearMaintenance NOTIFY totalWaterCostsYearMaintenanceChanged)


    // TOTAL
    Q_PROPERTY(double totalCostsYear READ totalCostsYear NOTIFY totalCostsYearChanged)
    Q_PROPERTY(double totalWaterCostsYear READ totalWaterCostsYear NOTIFY totalWaterCostsYearChanged)


public:
    explicit ROCosts(const ROSystem* const sys, QObject *parent = 0);
    ROCosts(); // by qml

    // SYSTEM
    double permeateYearAmount() const;
    double systemCost() const;
    double systemSetCostYear() const;
    double systemWithYearSetCost() const;

    double microfiltersMembraneCount() const; // User
    double microfiltersSystemCount() const;
    double microfiltersYearChangeFrequency() const; // User
    double microfiltersYearCount() const;
    double microfilterCost() const; // User
    double microfiltersYearCost() const;

    double membranesYearChangeFrequency() const; // User
    double membranesYearCount() const;
    double membraneCost() const;
    double membranesYearCost() const;

    double electricEnergyYearAmount() const;
    double electricEnergyCost() const;
    double electricEnergyYearCost() const;

    double totalCostsYearSystem() const;
    double totalWaterCostsYearSystem() const;


    // SCALING
    double washingSolutionVolume() const;
    double washingFrequency() const;
    double washingSolutionYearVolume() const;

    double citricAcidAmount() const;
    double citricAcidYearAmount() const;
    double citricAcidCost() const;
    double citricAcidYearCost() const;

    double H2SO4Amount() const;
    double H2SO4YearAmount() const;
    double H2SO4Cost() const;
    double H2SO4YearCost() const;

    double trilonBAmount() const;
    double trilonBYearAmount() const;
    double trilonBCost() const;
    double trilonBYearCost() const;

    double alkaliAmount() const;
    double alkaliYearAmount() const;
    double alkaliCost() const;
    double alkaliYearCost() const;

    double antiscalantAmount() const;
    double antiscalantSystemAmount() const;
    double antiscalantYearAmount() const;
    double antiscalantCost() const;
    double antiscalantYearCost() const;

    double acidAntiscalantAmount() const;
    double acidAntiscalantSystemAmount() const;
    double acidAntiscalantYearAmount() const;
    double acidAntiscalantCost() const;
    double acidAntiscalantYearCost() const;

    double totalCostsYearScaling() const;
    double totalWaterCostsYearScaling() const;


    // WATER
    double rawWaterYearAmount() const;
    double rawWaterCost() const;
    double rawWaterYearCost() const;

    double concentrateDropYearAmount() const;
    double concentrateDropCost() const;
    double concentrateDropYearCost() const;

    double totalCostsYearWater() const;
    double totalWaterCostsYearWater() const;


    // MAINTENANCE
    double staffCount() const;
    double staffSalary() const;
    double staffYearSalary() const;
    double staffYearCost() const;
    double accruals() const;
    double accrualsYear() const;
    double accrualsYearCost() const;
    double overheadRate() const;
    double systemLifetime() const;
    double deductionsYearCost() const;

    double totalCostsYearMaintenance() const;
    double totalWaterCostsYearMaintenance() const;

    double totalCostsYear() const;
    double totalWaterCostsYear() const;



    void setSystemCost(double value);

    // SYSTEM
    void setMicrofiltersMembraneCount(double value);
    void setMicrofiltersYearChangeFrequency(double value);
    void setMicrofilterCost(double value);

    void setMembranesYearChangeFrequency(double value);
    void setMembraneCost(double value);

    void setElectricEnergyCost(double value);


    // SCALING
    void setWashingFrequency(double value);

    void setCitricAcidCost(double value);
    void setH2SO4Cost(double value);
    void setTrilonBCost(double value);
    void setAlkaliCost(double value);

    void setAntiscalantAmount(double value);
    void setAntiscalantCost(double value);

    void setAcidAntiscalantAmount(double value);
    void setAcidAntiscalantCost(double value);

    // WATER
    void setRawWaterCost(double value);
    void setConcentrateDropCost(double value);

    // MAINTENANCE
    void setStaffCount(double value);
    void setStaffSalary(double value);
    void setOverheadRate(double value);
    void setAccruals(double value);

    void setSystemLifetime(double value);
    
signals:
    // SYSTEM
    void permeateYearAmountChanged();
    void systemCostChanged();
    void systemCostSetYearChanged();
    void systemWithYearSetCostChanged();

    void microfiltersMembraneCountChanged();
    void microfiltersSystemCountChanged();
    void microfiltersYearChangeFrequencyChanged();
    void microfiltersYearCountChanged();
    void microfilterCostChanged();
    void microfiltersYearCostChanged();

    void membranesYearChangeFrequencyChanged();
    void membranesYearCountChanged();
    void membraneCostChanged();
    void membranesYearCostChanged();

    void electricEnergyYearAmountChanged();
    void electricEnergyCostChanged();
    void electricEnergyYearCostChanged();

    void totalCostsYearSystemChanged();
    void totalWaterCostsYearSystemChanged();

    // SCALING
    void washingSolutionVolumeChanged();
    void washingFrequencyChanged();

    void citricAcidYearAmountChanged();
    void citricAcidCostChanged();
    void citricAcidYearCostChanged();

    void H2SO4YearAmountChanged();
    void H2SO4CostChanged();
    void H2SO4YearCostChanged();

    void trilonBYearAmountChanged();
    void trilonBCostChanged();
    void trilonBYearCostChanged();

    void alkaliYearAmountChanged();
    void alkaliCostChanged();
    void alkaliYearCostChanged();

    void antiscalantAmountChanged();
    void antiscalantSystemAmountChanged();
    void antiscalantCostChanged();
    void antiscalantYearCostChanged();

    void acidAntiscalantAmountChanged();
    void acidAntiscalantSystemAmountChanged();
    void acidAntiscalantCostChanged();
    void acidAntiscalantYearCostChanged();

    void totalCostsYearScalingChanged();
    void totalWaterCostsYearScalingChanged();

    // WATER
    void rawWaterYearAmountChanged();
    void rawWaterCostChanged();
    void rawWaterYearCostChanged();

    void concentrateDropYearAmountChanged();
    void concentrateDropCostChanged();
    void concentrateDropYearCostChanged();

    void totalCostsYearWaterChanged();
    void totalWaterCostsYearWaterChanged();


    // MAINTENANCE
    void staffCountChanged();
    void staffSalaryChanged();
    void staffYearSalaryChanged();
    void overheadRateChanged();
    void staffYearCostChanged();
    void accrualsChanged();
    void accrualsYearChanged();
    void accrualsYearCostChanged();
    void systemLifetimeChanged();
    void deductionsYearCostChanged();

    void totalCostsYearMaintenanceChanged();
    void totalWaterCostsYearMaintenanceChanged();

    // TOTAL
    void totalCostsYearChanged();
    void totalWaterCostsYearChanged();

public slots:
    void reset();
    void copyDataFrom(const ROCosts* const);

private slots:
    void updateSystemCost();

private:
    const ROSystem* const _sys;
    double _systemCost;

    double _microfiltersMembraneCount;
    double _microfiltersYearChangeFrequency;
    double _microfilterCost;

    double _membranesYearChangeFrequency;
    double _membraneCost;


    double _washingFrequency;
    double _citricAcidCost;
    double _H2SO4Cost;
    double _trilonBCost;
    double _alkaliCost;

    double _antiscalantAmount;
    double _antiscalantCost;
    double _acidAntiscalantAmount;
    double _acidAntiscalantCost;
    double _electricEnergyCost;

    double _rawWaterCost;
    double _concentrateDropCost;

    double _staffCount;
    double _staffSalary;
    double _accruals;
    double _overheadRate;

    double _systemLifetime;


};

#endif // ROCOSTS_H
