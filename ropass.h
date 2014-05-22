#ifndef ROPASS_H
#define ROPASS_H

#include <QList>
#include <QVector>
#include <QMap>
#include <QObject>

#include "roflow.h"
#include "rostage.h"
#include "roflowmixer.h"

#include "ropassparamsetcontroller.h"

#include "rowarning.h"

/* содержит информацию: СИФ, коэф. поправки, данные по потокам,
 * данные по рециклу на себя и др., а также информацию о стадиях (ROStage).
 */


class ROSystem;
class ROPass : public ROAbstractElement {

    Q_OBJECT
    Q_ENUMS(ParamSetState)

    Q_PROPERTY(int stageCount READ stageCount NOTIFY stageCountChanged)
    Q_PROPERTY(int elementsCount READ elementsCount NOTIFY elementsCountChanged)

    Q_PROPERTY(double activeArea READ activeArea NOTIFY totalActiveAreaChanged)
    Q_PROPERTY(double recovery READ recovery WRITE setRecovery NOTIFY recoveryChanged)
    Q_PROPERTY(double flowFactor READ flowFactor /*WRITE setFlowFactor*/ NOTIFY flowFactorChanged)
    Q_PROPERTY(double saltPassageYearIncrease READ saltPassageYearIncrease /*WRITE setFlowFactor*/ NOTIFY saltPassageYearIncreaseChanged)
//    Q_PROPERTY(double flowFactor READ flowFactor /*WRITE setFlowFactor*/ NOTIFY flowFactorChanged)

    Q_PROPERTY(double averageFlux READ averageFlux NOTIFY averageFluxChanged)
    Q_PROPERTY(double power READ power NOTIFY powerChanged)
    Q_PROPERTY(double specificEnergy READ specificEnergy NOTIFY specificEnergyChanged)


    Q_PROPERTY(ROStage* firstStage READ firstStage NOTIFY firstStageChanged) // меняется при смене pass
    Q_PROPERTY(ROStage* lastStage READ lastStage NOTIFY lastStageChanged) // меняется при смене pass


    // FLOWS
    Q_PROPERTY(ROFlow* feed READ feed WRITE setFeed NOTIFY feedChanged)
    Q_PROPERTY(ROFlow* blendedRecycledFeed READ blendedRecycledFeed CONSTANT)
    Q_PROPERTY(ROFlow* firstStageFeed READ firstStageFeed CONSTANT)
    Q_PROPERTY(ROFlow* permeate READ permeate CONSTANT)
    Q_PROPERTY(ROFlow* totalStagesPermeate READ totalStagesPermeate CONSTANT)
    Q_PROPERTY(ROFlow* concentrate READ concentrate CONSTANT)


    // RECYCLE
    Q_PROPERTY(double selfRecycle READ selfRecycle WRITE setSelfRecycle NOTIFY selfRecycleChanged)
    Q_PROPERTY(bool hasSelfRecycle READ hasSelfRecycle NOTIFY hasSelfRecycleChanged)

    // BLEND PERMEATE
    Q_PROPERTY(double blendPermeate READ blendPermeate WRITE setBlendPermeate NOTIFY blendPermeateChanged)
    Q_PROPERTY(bool hasBlendPermeate READ hasBlendPermeate NOTIFY hasBlendPermeateChanged)

    // STAGES CONSTANTS
    Q_PROPERTY(int MAX_STAGES_COUNT READ MAX_STAGES_COUNT CONSTANT)
    Q_PROPERTY(int MIN_STAGES_COUNT READ MIN_STAGES_COUNT CONSTANT)
    Q_PROPERTY(int MAX_RECOVERY READ MAX_RECOVERY CONSTANT)
    Q_PROPERTY(int MIN_RECOVERY READ MIN_RECOVERY CONSTANT)

public:
    static int MAX_STAGES_COUNT() { return _MAX_STAGES_COUNT; }
    static int MIN_STAGES_COUNT() { return _MIN_STAGES_COUNT; }
    static double MAX_RECOVERY() { return _MAX_RECOVERY; }
    static double MIN_RECOVERY() { return _MIN_RECOVERY; }

    explicit ROPass(ROSystem* const system, ROFlow* const feed = 0);
    explicit ROPass();  // только для QML
    ~ROPass();
    ROPass* clone(ROFlow* const newFeed = 0);

    ROSystem* system() const;

    ROFlow* const feed() const;
    ROFlow* const blendedRecycledFeed() const;
    ROFlow* const firstStageFeed() const;
    ROFlow* const permeate() const;
    ROFlow* const totalStagesPermeate() const;
    ROFlow* const concentrate() const;
    double recovery() const;
    double flowFactor() const;
    double saltPassageYearIncrease() const;
    double blendPermeate() const;
    double power() const;
    double specificEnergy() const;
    double activeArea() const;

    int stageCount() const;
    int elementsCount() const;
    Q_INVOKABLE ROStage* stage(int stageNumber) const;
    Q_INVOKABLE int stageIndex(ROStage* stage) const;
    Q_INVOKABLE ROStage* addStage(int copyFromStageNumber = -1);
    Q_INVOKABLE bool removeStage(int stageNumber = -1);
    bool setStageCount(int stageCount);
    ROStage* const firstStage() const;
    ROStage* const lastStage() const;


    void setRecovery(double value);
    void setFeed(ROFlow* const newFeed);
    // void setFlowFactor(double value);
    void setBlendPermeate(double value);
    void setSelfRecycle(double value);
    Q_INVOKABLE void removeBlendPermeate();


    const QMap<int, double> outgoingRecycles() const;
    const QMap<int, double> incomingRecycles() const;
    Q_INVOKABLE double recycle(int recyclePassNumber) const;
    Q_INVOKABLE void addRecycle(int toPassIdx, double rate);
    Q_INVOKABLE void addRecycle(const ROPass* const toPass, double rate);
    Q_INVOKABLE void removeRecycle(int toPassIdx);
    Q_INVOKABLE void removeRecycle(const ROPass* const toPass);
    double selfRecycle() const;
    bool hasSelfRecycle() const;
    Q_INVOKABLE bool hasRecycle(int toPassIdx) const;
    Q_INVOKABLE bool hasRecycle(const ROPass* const toPass) const;
    Q_INVOKABLE void removeSelfRecycle();


    bool hasBlendPermeate() const;
    void copyDataFrom(const ROPass* const);

private:
    ROSystem* const _system;
    QList<ROStage*> _stages;
    double _recovery;
    // double _flowFactor;
    bool _flowChanging;
    bool _permeateChanging;
    ROFlow* _feed;
    ROFlow* const _blendedRecycledFeed;
    ROFlow* const _firstStageFeed;
    ROFlow* const _permeate;
    ROFlow* const _totalStagesPermeate;
    ROFlow* const _concentrate;
    double _selfRecycle;
    double _blendPermeate;

    bool _hasSelfRecycle;
    bool _hasBlendPermeate;

    // CONSTANTS
    static const int _MAX_STAGES_COUNT;
    static const int _MIN_STAGES_COUNT;
    static const double _MAX_RECOVERY;
    static const double _MIN_RECOVERY;


signals:
    void totalActiveAreaChanged();
    void recoveryChanged();
    void averageFluxChanged();

    void hasSelfRecycleChanged();
    void hasBlendPermeateChanged();
    void recycleChanged(); // любые изменения в рециклах, кроме собственного

    void flowFactorChanged();
    void saltPassageYearIncreaseChanged();
    void blendPermeateChanged();
    void selfRecycleChanged();
    void powerChanged();
    void specificEnergyChanged();
    void stageCountChanged();
    void beginFeedChange();
    void feedChanged();

    void firstStageChanged();
    void lastStageChanged();

    void elementsCountChanged();

private slots:
    void updateBlendPermeate();
    void updateRecycles();

public slots:
    void reset();
};

#endif // ROPASS_H
