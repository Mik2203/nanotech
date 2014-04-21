#ifndef ROELEMENT_H
#define ROELEMENT_H

#include <QtPlugin>
#include "roflow.h"

#include <QObject>

// базовый класс для элементов схемы
class ROAbstractElement: public QObject {
public:
    explicit ROAbstractElement(): QObject() {}
    virtual ~ROAbstractElement() {}
    
    virtual ROFlow* const feed() const =0;
    virtual ROFlow* const permeate() const =0;
    virtual ROFlow* const concentrate() const =0;

    virtual double activeArea() const =0;
    virtual double recovery() const =0;
    virtual double averageFlux() const;
signals:
    
public slots:
    
};

//Q_DECLARE_INTERFACE(ROElement, "ROElement/1.0")

#endif // ROELEMENT_H
