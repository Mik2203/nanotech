#ifndef ROSOLUTES_H
#define ROSOLUTES_H

#include <QObject>
#include <QVector>

/* содержит информацию по ионам,
 * такую как солесодержание, ph, температура, LSI, S&SDI и др.
 */

class ROSolutes : public QObject {

    Q_OBJECT
    Q_ENUMS(ScalingCompound)
    Q_ENUMS(Solutes)

    Q_PROPERTY(double pH READ pH WRITE setPH NOTIFY pHChanged)
    Q_PROPERTY(double temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(double totalValueMgl READ totalValueMgl NOTIFY solutesChanged)
    Q_PROPERTY(double ionicStrength READ ionicStrength NOTIFY solutesChanged)
    Q_PROPERTY(double ionicStrength2 READ ionicStrength2 NOTIFY solutesChanged)
    Q_PROPERTY(double lsi READ lsi NOTIFY solutesChanged)
    Q_PROPERTY(double sdsi READ sdsi NOTIFY solutesChanged)

    Q_PROPERTY(bool isBalanced READ isBalanced NOTIFY solutesChanged)
    Q_PROPERTY(bool isFilled READ isFilled NOTIFY solutesChanged)

public:
    enum Solutes { NH4, K, Na, Mg, Ca, Fe, Ba, Sr, TotalCations,
                   Cl = TotalCations, NO3, F, SO4, CO3, HCO3, TotalAnions,
                   CO2 = TotalAnions, B, SiO2, TotalIons, H = TotalIons };


    enum Types {
        Anion =  0x001,
        Cation = 0x010,
        Other =  0x100,
        Any =    0x111
    };

    enum ScalingCompound { CaSO4, BaSO4, SrSO4, CaF2, SiO2_, MgOH2, TotalCompounds };

    enum Units { Mgl, Meql, MeqKg, MolL };

    struct SoluteInfo {
        SoluteInfo(Types type_, double molarMass_, int ionicCharge_): type(type_), ionicCharge(ionicCharge_), molarMass(molarMass_) {}
        Types type;
        double molarMass;
        double ionicCharge;
    };

    static const SoluteInfo soluteInfos[];

    explicit ROSolutes(QObject *parent = 0);
    
    double pH() const;
    double temperature() const;
    double ionicStrength() const;
    double ionicStrength2() const;
    double lsi() const;
    double sdsi() const;

    double hamma(int si) const;


    static double valueFromTotal(double total, int si);
    Q_INVOKABLE double saturation(ScalingCompound compound) const;
    static double totalMolarMass(Types type = Any);
    double totalValue(Units units, Types type = Any) const;
    double totalValueMgl() const; // FOR PROPERTY

    static double molarMass(int si);
    static double ionicCharge(int si);
    Q_INVOKABLE double mgl(int soluteIndex) const;
    Q_INVOKABLE double meql(int soluteIndex) const;
    Q_INVOKABLE double value(int soluteIndex, ROSolutes::Units units) const;
    Q_INVOKABLE bool isIonFilled(int soluteIndex) const;
    Q_INVOKABLE bool isSaturated(int compound) const;
    Q_INVOKABLE static bool isEditable(int soluteIndex);
    QVector<int> filledIons() const;
    QVector<int> saturatedCompounds() const;


    bool isBalanced() const;
    bool isFilled() const;



    void setPH(double value);
    void setTemperature(double value);
    void setMgl(int soluteIndex, double mgl);
    void setMeql(int soluteIndex, double meql);
    void setValue(int soluteIndex, double value, ROSolutes::Units units);

    void addValue(int soluteIndex, double value, ROSolutes::Units units);

    void setTotalValue(double value, Units units, Types type = Any);

private:
    QVector<double> _solutesMeql;
    double _pH;
    double _temperature;
    double _totalMolarMass;

    bool _changing;
    void calcAutoValues(); // расчет значений, зависящих от других

signals:
    void solutesChanged();
    void pHChanged();
    void temperatureChanged();
public slots:
    void beginChange();
    void endChange();
    void adjust();
    void reset(); //Types type = Any
    void recalculate();
    
};

#endif // ROSOLUTES_H

