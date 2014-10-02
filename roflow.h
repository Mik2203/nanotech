#ifndef ROFLOW_H
#define ROFLOW_H

#include <QList>

#include "rosolutes.h"
#include "rosolutemodel.h"
//#include "roflowrateparameter.h"

class ROSourceElement;
class RODestinationElement;
class ROFilterElement;

/* содержит информацию о потоке: объем, давление,
 * осмотическое давление, а также данные по ионам (ROSolutes).
 */

class ROFlow: public QObject {

    Q_OBJECT
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(double part READ part WRITE setPart NOTIFY rateChanged)  // alias to rate
    Q_PROPERTY(double temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(double pH READ pH WRITE setPH NOTIFY pHChanged)

    Q_PROPERTY(double pressure READ pressure WRITE setPressure NOTIFY pressureChanged)
    Q_PROPERTY(double osmoticPressure READ osmoticPressure NOTIFY osmoticPressureChanged)
    Q_PROPERTY(ROSolutes* solutes READ solutes CONSTANT)



public:
    ROFlow();
    ~ROFlow();

    ROSolutes* const solutes() const;
    double rate() const;
    double part() const;
    double temperature() const { return solutes()->temperature(); }
    double pH() const { return solutes()->pH(); }
    double pressure() const;
    double osmoticPressure() const;



    void setRate(double rate);
    void setPart(double part);
    void setTemperature(double value) { solutes()->setTemperature(value); }
    void setPH(double value) { solutes()->setPH(value); }
    void setPressure(double value);


    void reset();
    void copySolutesFrom(const ROFlow* other);
    void mix(const ROFlow* other);
    void subtract(const ROFlow* other);
    void copyDataFrom(const ROFlow* other);

    ROFlow* clone();


private:
    ROSolutes* _solutes;
    double _rate;
    double _pressure;
    double _osmoticPressure;

    Q_DISABLE_COPY(ROFlow)

private slots:
    void updateOsmoticPressure();

signals:
    void temperatureChanged();
    void pHChanged(); // FOR USABILITY ONLY
    void solutesChanged(); // FOR USABILITY ONLY
    void rateChanged();
    void pressureChanged();
    void osmoticPressureChanged();
};
#endif // ROFLOW_H
