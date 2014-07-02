#ifndef ROVESSEL_H
#define ROVESSEL_H

#include <QObject>

#include "roflow.h"
#include "romembrane.h"
#include "roabstractelement.h"

/* содержит информацию об элемете системы такую как СИФ,
 * активная площадь, данные по потокам.
 */

class ROStage;
class ROElement : public ROAbstractElement
{
    Q_OBJECT

    Q_PROPERTY(double activeArea READ activeArea NOTIFY activeAreaChanged)

    Q_PROPERTY(double recovery READ recovery NOTIFY recoveryChanged)
    Q_PROPERTY(double averageFlux READ averageFlux NOTIFY averageFluxChanged)

    // FLOWS
    Q_PROPERTY(ROFlow* permeate READ permeate CONSTANT)
    Q_PROPERTY(ROFlow* feed READ feed WRITE setFeed NOTIFY feedChanged)
    Q_PROPERTY(ROFlow* concentrate READ concentrate CONSTANT)

public:
    explicit ROElement(ROStage* stage, ROFlow* feed = 0);
    ROElement(); // Только для QML
    ~ROElement();
    
    ROFlow* const feed() const;
    ROFlow* const permeate() const;
    ROFlow* const concentrate() const;

    void setFeed(ROFlow* const newFeed);

    ROStage* const stage() const;

    double activeArea() const;
    double recovery() const;

    ROElement* clone(ROFlow* newFeed = 0);
    //void copyDataFrom(const ROStage* const);

private:
    ROFlow* _feed;
    ROFlow* const _permeate;
    ROFlow* const _concentrate;

    ROStage* _stage;

signals:
    void activeAreaChanged();
    void recoveryChanged();
    void feedChanged();
    void averageFluxChanged();
    
};

#endif // ROVESSEL_H
