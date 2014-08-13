#ifndef ROMEMBRANE_H
#define ROMEMBRANE_H
#include <QObject>
#include "romembranesize.h"

struct SPmCoeffsStruct { double a,b,c; };

/* класс-контейнер для представления мембранного элемента.
 */

class ROMembrane : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString series READ series CONSTANT)
    Q_PROPERTY(QString model READ model CONSTANT)
    Q_PROPERTY(QString seriesAndModel READ seriesAndModel CONSTANT)
    Q_PROPERTY(double activeArea READ activeArea CONSTANT)
    Q_PROPERTY(double productivity READ productivity CONSTANT)
    Q_PROPERTY(double rejection READ rejection CONSTANT)
    Q_PROPERTY(double pressure READ pressure CONSTANT)

public:
    explicit ROMembrane(int seriesIndex, const QString& series, const QString& model, const ROMembraneSize& membraneSize,
               double activeArea, double productivity,
               double rejection, double pressure);
    ROMembrane(); // for QML
    ROMembrane(const ROMembrane&);
    const QString& series() const;
    const QString& model() const;
    int seriesIndex() const;
    const QString seriesAndModel() const;
    const ROMembraneSize membraneSize() const;
    double activeArea() const;
    double productivity() const;
    double rejection() const;
    double pressure() const;

    static SPmCoeffsStruct SPmCoeffs(int series, int si);
    static SPmCoeffsStruct PhSPmCoeffs(int series);
    static double PhSPmMax(int series);
    static double PhSPmMin(int series);
    static double aCoeff(int series);
private:
    int _seriesIndex;
    QString _series;
    QString _model;
    ROMembraneSize _membraneSize;
    double _activeArea;
    double _productivity;
    double _rejection;
    double _pressure;

    static SPmCoeffsStruct _SPmCoeffs[];
    static SPmCoeffsStruct _PhSPmCoeffs[];
    static double _PhSPmMax[];
    static double _PhSPmMin[];
    static double _aCoeff[];
};

#endif // ROMEMBRANE_H
