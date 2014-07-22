#ifndef ROUNITS_H
#define ROUNITS_H

#include <QObject>
#include "singleton.h"

/* класс, отвечающий за установку текущих единиц измерения,
 * а также обеспечивающий манипуляции по их преобразованию.
 */

#define roUnits ROUnits::instance()
class ROUnits : public QObject, public Singleton<ROUnits> {

    Q_OBJECT
    Q_ENUMS(FlowUnits)
    Q_ENUMS(FluxUnits)
    Q_ENUMS(AreaUnits)
    Q_ENUMS(PressureUnits)
    Q_ENUMS(TemperatureUnits)

    Q_PROPERTY(FlowUnits flowUnits READ flowUnits WRITE setFlowUnits NOTIFY flowUnitsChanged)
    Q_PROPERTY(FluxUnits fluxUnits READ fluxUnits WRITE setFluxUnits NOTIFY fluxUnitsChanged)
    Q_PROPERTY(AreaUnits areaUnits READ areaUnits NOTIFY areaUnitsChanged)
    Q_PROPERTY(PressureUnits pressureUnits READ pressureUnits WRITE setPressureUnits NOTIFY pressureUnitsChanged)
    Q_PROPERTY(TemperatureUnits temperatureUnits READ temperatureUnits WRITE setTemperatureUnits NOTIFY temperatureUnitsChanged)

    Q_PROPERTY(FlowUnits DEFAULT_FLOW_UNITS READ DEFAULT_FLOW_UNITS CONSTANT)
    Q_PROPERTY(FluxUnits DEFAULT_FLUX_UNITS READ DEFAULT_FLUX_UNITS CONSTANT)
    Q_PROPERTY(AreaUnits DEFAULT_AREA_UNITS READ DEFAULT_AREA_UNITS CONSTANT)
    Q_PROPERTY(PressureUnits DEFAULT_PRESSURE_UNITS READ DEFAULT_PRESSURE_UNITS CONSTANT)
    Q_PROPERTY(TemperatureUnits DEFAULT_TEMPERATURE_UNITS READ DEFAULT_TEMPERATURE_UNITS CONSTANT)


public:
    enum FlowUnits { m3h, m3d, GPD, GPM, FLOW_UNIT_COUNT };
    enum FluxUnits { lmh, GFD, FLUX_UNIT_COUNT };
    enum AreaUnits { m2, ft2, AREA_UNIT_COUNT };
    enum PressureUnits { bar, PSI, MPa, atm, PRESSURE_UNIT_COUNT };
    enum TemperatureUnits { Celsius, Fahrenheit, TEMPERATURE_UNITS_COUNT };



    FlowUnits flowUnits() const;
    FluxUnits fluxUnits() const;
    AreaUnits areaUnits() const;
    PressureUnits pressureUnits() const;
    TemperatureUnits temperatureUnits() const;



    void setFlowUnits(FlowUnits);
    void setFluxUnits(FluxUnits);
    void setPressureUnits(PressureUnits);
    void setTemperatureUnits(TemperatureUnits);
    
signals:
    void flowUnitsChanged();
    void fluxUnitsChanged();
    void areaUnitsChanged();
    void pressureUnitsChanged();
    void temperatureUnitsChanged();
    
public slots:
    static double convertFlowUnits(double value, FlowUnits from = DEFAULT_FLOW_UNITS(), FlowUnits to = roUnits->flowUnits());
    static double convertFluxUnits(double value, FluxUnits from = DEFAULT_FLUX_UNITS(), FluxUnits to = roUnits->fluxUnits());
    static double convertAreaUnits(double value, AreaUnits from = DEFAULT_AREA_UNITS(), AreaUnits to = roUnits->areaUnits());
    static double convertPressureUnits(double value, PressureUnits from = DEFAULT_PRESSURE_UNITS(), PressureUnits to = roUnits->pressureUnits());
    static double convertTemperatureUnits(double value, TemperatureUnits from = DEFAULT_TEMPERATURE_UNITS(), TemperatureUnits to=roUnits->temperatureUnits());

    static FlowUnits DEFAULT_FLOW_UNITS();
    static FluxUnits DEFAULT_FLUX_UNITS();
    static AreaUnits DEFAULT_AREA_UNITS();
    static PressureUnits DEFAULT_PRESSURE_UNITS();
    static TemperatureUnits DEFAULT_TEMPERATURE_UNITS();

private:
    FlowUnits _flowUnits;
    FluxUnits _fluxUnits;
    AreaUnits _areaUnits;
    PressureUnits _pressureUnits;
    TemperatureUnits _temperatureUnits;

protected:
    explicit ROUnits(FlowUnits flowU = m3h,
                     FluxUnits fluxU = lmh,
                     AreaUnits areaU = m2,
                     PressureUnits pressU = bar,
                     TemperatureUnits tempU = Celsius,
                     QObject *parent = 0);
    friend class Singleton<ROUnits>;
    ~ROUnits() {}

};

#endif // ROUNITS_H
