#ifndef ROUNITSTEXT_H
#define ROUNITSTEXT_H

#include <QObject>
#include <QVariant>
#include "rounits.h"
#include "singleton.h"

// Класс ROUnitsText обеспечивает поддержку перевода ROUnits.

#define roUnitsText ROUnitsText::instance()
class ROUnitsText : public QObject, public Singleton<ROUnitsText> {
    Q_OBJECT
    Q_PROPERTY(QVariant flowUnitsTextList READ flowUnitsTextList NOTIFY textUpdated)
    Q_PROPERTY(QVariant fluxUnitsTextList READ fluxUnitsTextList NOTIFY textUpdated)
    Q_PROPERTY(QVariant pressureUnitsTextList READ pressureUnitsTextList NOTIFY textUpdated)
    Q_PROPERTY(QVariant temperatureUnitsTextList READ temperatureUnitsTextList NOTIFY textUpdated)

public:
    QVariant flowUnitsTextList();
    QVariant fluxUnitsTextList();
    QVariant pressureUnitsTextList();
    QVariant temperatureUnitsTextList();


    
signals:
    void textUpdated();
    
public slots:
    // Can't pass enums as parameters, because of QML limitations
    QString flowUnitText(int); // int = ROUnits::FlowUnits
    QString fluxUnitText(int); // int = ROUnits::FluxUnits
    QString pressureUnitText(int); //int = ROUnits::PressureUnits
    QString temperatureUnitText(int); // int = ROUnits::TemperatureUnits
    void update();

private:
    QVariant _flowUnitsTextList;
    QVariant _fluxUnitsTextList;
    QVariant _pressureUnitsTextList;
    QVariant _temperatureUnitsTextList;

    static const char *_flowUnitsData[];
    static const char *_fluxUnitsData[];
    static const char *_pressureUnitsData[];
    static const char *_temperatureUnitsData[];
    

protected:
    explicit ROUnitsText(QObject *parent = 0);
    friend class Singleton<ROUnitsText>;
    ~ROUnitsText();
};

#endif // ROUNITSTEXT_H
