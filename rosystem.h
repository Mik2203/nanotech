#ifndef ROSYSTEM_H
#define ROSYSTEM_H

#include <QList>
#include <QSharedPointer>
#include <QObject>
#include <QtDeclarative/QDeclarativeListProperty>

//#include "rofeedelement.h"
//#include "rofilterelement.h"
//#include "rofilterelementposition.h"
#include "roflow.h"
#include "rofeed.h"
#include "ropass.h"
#include "roflowmixer.h"
#include "roabstractelement.h"
#include "roscalingelement.h"

#include "rowatertypemodel.h"
#include "romembranesmodel.h"



/* содержит данные о системе, такие как температура, тип воды,
 * а также данные по ступеням (ROPass), рециклам между ступенями,
 * данные по входным потокам (ROFeed),
 * а также данные по осадкообразованию (ROScalingElement).
 */

class ROSystem: public ROAbstractElement {

    Q_OBJECT

    Q_PROPERTY(double temperature READ temperature /*WRITE setTemperature*/ NOTIFY temperatureChanged)
    Q_PROPERTY(double recovery READ recovery NOTIFY recoveryChanged)
    Q_PROPERTY(double activeArea READ activeArea NOTIFY totalActiveAreaChanged)
    Q_PROPERTY(double averageFlux READ averageFlux NOTIFY averageFluxChanged)
    Q_PROPERTY(double power READ power NOTIFY powerChanged)
    Q_PROPERTY(double specificEnergy READ specificEnergy NOTIFY specificEnergyChanged)
    Q_PROPERTY(ROFlow* permeate READ permeate NOTIFY permeateChanged) // меняется при смене pass
    Q_PROPERTY(ROFlow* concentrate READ concentrate CONSTANT)

    Q_PROPERTY(double flowFactor READ flowFactor NOTIFY waterTypeIndexChanged)

    Q_PROPERTY(ROPass* firstPass READ firstPass NOTIFY firstPassChanged)
    Q_PROPERTY(ROPass* lastPass READ lastPass NOTIFY lastPassChanged)

    Q_PROPERTY(double pH READ pH NOTIFY pHChanged)

    Q_PROPERTY(ROScalingElement* scalingElement READ scalingElement CONSTANT)

    Q_PROPERTY(int waterTypeIndex READ waterTypeIndex WRITE setWaterTypeIndex NOTIFY waterTypeIndexChanged)




    Q_PROPERTY(int passCount READ passCount WRITE setPassCount NOTIFY passCountChanged)
    Q_PROPERTY(int feedCount READ feedCount NOTIFY feedCountChanged)
    Q_PROPERTY(int elementsCount READ elementsCount NOTIFY elementsCountChanged)
    Q_PROPERTY(int stagesCount READ stagesCount NOTIFY stagesCountChanged)

    Q_PROPERTY(ROFlow* feed READ feed CONSTANT)
    Q_PROPERTY(ROFlow* adjustedFeed READ adjustedFeed CONSTANT)

    Q_PROPERTY(int MAX_PASSES_COUNT READ MAX_PASSES_COUNT CONSTANT)
    Q_PROPERTY(int MIN_PASSES_COUNT READ MIN_PASSES_COUNT CONSTANT)
    Q_PROPERTY(int MAX_FEEDS_COUNT READ MAX_FEEDS_COUNT CONSTANT)
    Q_PROPERTY(int MIN_FEEDS_COUNT READ MIN_FEEDS_COUNT CONSTANT)

public:
    enum Parameters { PassesCount, ParametersCount };
    ROSystem();
    ~ROSystem();

    void copyDataFrom(const ROSystem* const);
    void copySystemDataFrom(const ROSystem* const);

    int feedCount() const;
    ROFlow* const feed() const;
    ROFlow* const adjustedFeed() const;
    Q_INVOKABLE ROFeed* partFeed(int feedFlowIndex) const;
    Q_INVOKABLE ROFeed* addPartFeed(int copyFromFeedIndex = -1);
    Q_INVOKABLE bool removePartFeed(int feedIndex = -1);
    Q_INVOKABLE int partFeedIndex(ROFeed* feed) const;
    ROFeed* firstPartFeed() const; // TODO PROPERTY
    ROFeed* lastPartFeed() const; // TODO PROPERTY
    bool setFeedCount(int feedCount);

    int passCount() const;
    Q_INVOKABLE ROPass* pass(int passIndex) const;
    ROPass* firstPass() const;
    ROPass* lastPass() const;
    Q_INVOKABLE int passIndex(ROPass* pass) const;
    int passIndex(const ROPass* const pass) const;
    Q_INVOKABLE ROPass* addPass(int copyFromPassNumber = -1);
    Q_INVOKABLE bool removePass(int passNumber = -1);
    bool setPassCount(int passCount);

    int elementsCount() const;
    int stagesCount() const;

    double temperature() const;
    double pH() const;
    double recovery() const;
    double power() const;
    double specificEnergy() const;
    ROFlow* const permeate() const;
    ROFlow* const concentrate() const;
    double activeArea() const;
    int waterTypeIndex() const;
    void setWaterTypeIndex(int index);
    ROScalingElement* const scalingElement() const;

    double flowFactor() const;
    double permeateFlowFactor() const;

    int totalRecycleCount() const;

    const QMap<int, double> passIncomingRecycles(int toPassIdx) const;
    const QMap<int, double> passOutgoingRecycles(int fromPassIdx) const;

    double recycle(int fromPassIdx, int toPassIdx) const;
    double recycle(const ROPass* const fromPass, const ROPass* const toPass) const;

    void setPassRecycle(int fromPassIdx, int toPassIdx, double value);
    void setPassRecycle(const ROPass* const fromPass, const ROPass* const toPass, double value);

    void removePassRecycle(int fromPassIdx, int toPassIdx = -1); // -1 = ALL
    void removePassRecycle(const ROPass* const fromPass, const ROPass* const toPass);

    bool hasRecycle(int fromPassIdx, int toPassIdx) const;
    bool hasRecycle(const ROPass* const fromPass, const ROPass* const toPass) const;

    QVector<int> filledIons() const;
    QVector<int> saturatedCompounds() const;

public slots:
    static int MAX_PASSES_COUNT() { return _MAX_PASSES_COUNT; }
    static int MIN_PASSES_COUNT() { return _MIN_PASSES_COUNT; }
    static int MAX_FEEDS_COUNT() { return _MAX_FEEDS_COUNT; }
    static int MIN_FEEDS_COUNT() { return _MIN_FEEDS_COUNT; }

private:
    QList<ROFeed*> _feeds;
    QList<ROPass*> _passes;
    ROFlowMixer* _feedsToResultFeed_STH;
    ROFlowMixer* _adjustedFeedToResultFeed_R;
    ROScalingElement* _scalingElement;

    ROFlow* const _concentrate;
    ROFlow* _resultFeed;

    QMap<int, QMap<int, double> > _passRecycles; // from, to; лучше бы сделать структурой

    int _waterTypeIndex;
    // double _flowFactor;

    static const int _MAX_PASSES_COUNT;
    static const int _MIN_PASSES_COUNT;
    static const int _MAX_FEEDS_COUNT;
    static const int _MIN_FEEDS_COUNT;

signals:
    void temperatureChanged();
    void recoveryChanged();
    void averageFluxChanged();
    void totalActiveAreaChanged();

    void powerChanged();
    void specificEnergyChanged();
    void pHChanged();
    void passCountChanged();
    void feedCountChanged();
    void permeateChanged();
    void waterTypeIndexChanged();

    void firstPassChanged();
    void lastPassChanged();

    void elementsCountChanged();
    void stagesCountChanged();

private slots:
    void refreshPermeate();

public slots:
    void reset();
    void resetSystem();

};

#endif // ROSYSTEM_H
