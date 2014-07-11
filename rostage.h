#ifndef ROSTAGE_H
#define ROSTAGE_H

#include <QObject>
#include "roflow.h"
#include "romembrane.h"
#include "roelement.h"
#include "roabstractelement.h"



/* содержит информацию о стадии такую как модель мембранного элемента,
 * кол-во корпусов, ко-во элементов, данные по потокам стадии и др,
 * а также информацию об элементах системы (ROElement).
 *
 * параметр rate у фильтрата рассчитывается не здесь, а в классе "ROPass".
 * это связано с тем, что он устанавливается у ступени, а не у стадии.
 * А затем  распределяется по стадиям.
 *
 * Параметры концентрата рассчитываются в методе calculate() у солвера.
 */

class ROPass;
class ROStage: public ROAbstractElement {

    Q_OBJECT

    Q_PROPERTY(double activeArea READ activeArea NOTIFY totalActiveAreaChanged)
    Q_PROPERTY(double recovery READ recovery NOTIFY recoveryChanged)
    Q_PROPERTY(double averageFlux READ averageFlux NOTIFY averageFluxChanged)

    // FLOWS
    Q_PROPERTY(ROFlow* permeate READ permeate CONSTANT)
    Q_PROPERTY(ROFlow* feed READ feed WRITE setFeed NOTIFY feedChanged)
    Q_PROPERTY(ROFlow* concentrate READ concentrate CONSTANT)

    // MEMBRANE
    Q_PROPERTY(int membraneIndex READ membraneIndex WRITE setMembraneIndex NOTIFY membraneIndexChanged)
    Q_PROPERTY(ROMembrane* membrane READ membrane NOTIFY membraneIndexChanged)

    // VESSELS
    Q_PROPERTY(int vesselCount READ vesselCount WRITE setVesselCount NOTIFY vesselsCountChanged)
    Q_PROPERTY(int elementsPerVesselCount READ elementsPerVesselCount WRITE setElementsPerVesselCount NOTIFY elementsPerVesselCountChanged)
    Q_PROPERTY(int elementsCount READ elementsCount NOTIFY elementsCountChanged)
    Q_PROPERTY(ROElement* firstElement READ firstElement CONSTANT)
    Q_PROPERTY(ROElement* lastElement READ lastElement CONSTANT)

    // PRESSURE
    Q_PROPERTY(double preStagePressure READ preStagePressure WRITE setPreStagePressure NOTIFY preStagePressureChanged)
    Q_PROPERTY(double backPressure READ backPressure WRITE setBackPressure NOTIFY backPressureChanged)


public:
    static const int MAX_ELEMENTS_PER_VESSEL;
    static const int MIN_ELEMENTS_PER_VESSEL;
    static const int MAX_VESSELS_COUNT;
    static const int MIN_VESSELS_COUNT;

    explicit ROStage(ROPass* pass, ROFlow* feed = 0,
            int elementsPerVesselCount = MIN_ELEMENTS_PER_VESSEL, int vesselCount = MIN_VESSELS_COUNT);

    ROStage(); // Только для QML

    ~ROStage();
    ROFlow* const feed() const;
    ROFlow* const permeate() const;
    ROFlow* const concentrate() const;

    int vesselCount() const;
    int elementsPerVesselCount() const;
    int elementsCount() const;
    ROMembrane* const membrane() const;
    int membraneIndex() const;


    void setVesselCount(int vesselCount);

    void setElementsPerVesselCount(int elementsPerVesselCount);
    Q_INVOKABLE ROElement* element(int elementIndex) const;
    Q_INVOKABLE int elementIndex(ROElement* element) const;
    ROElement* const firstElement() const;
    ROElement* const lastElement() const;


    void setMembraneIndex(int membraneIndex);
    void setFeed(ROFlow* const newFeed);

    ROPass* const pass() const;

    double preStagePressure() const;
    void setPreStagePressure(double value);


    double backPressure() const;
    void setBackPressure(double value);

    double activeArea() const;
    double recovery() const;

    ROStage* clone(ROFlow* newFeed = 0);
    void copyDataFrom(const ROStage* const);

private:
    ROFlow* _feed;
    ROFlow* const _firstElementFeed;
    ROFlow* const _permeate;
    ROFlow* const _concentrate;
    ROMembrane* _membrane;

    ROPass* _pass;
    int _vesselCount;
    QList<ROElement*> _elements;
    int _membraneIndex;
    double _preStagePressure;
    double _backPressure;

signals:
    void totalActiveAreaChanged();
    void recoveryChanged();
    void vesselsCountChanged();
    void elementsPerVesselCountChanged();
    void elementsCountChanged();
    void feedChanged();
    void preStagePressureChanged();
    void backPressureChanged();
    void numberChanged();
    void membraneIndexChanged();
    void averageFluxChanged();

public slots:
    void reset();
};

#endif // ROSTAGE_H
