#ifndef ROSCALINGELEMENT_H
#define ROSCALINGELEMENT_H

#include <QObject>

#include "roflow.h"

/* содержит информацию и методы обработки
 * для различных корректировок входного потока.
 */


class ROScalingElement : public QObject
{
    Q_OBJECT
    Q_ENUMS(FeedAdjustment)
    Q_ENUMS(Acid)
    Q_PROPERTY(ROFlow* feed READ feed WRITE setFeed NOTIFY feedChanged)
    Q_PROPERTY(FeedAdjustment adjustment READ adjustment WRITE setAdjustment NOTIFY adjustmentChanged)
    Q_PROPERTY(ROFlow* adjustedFeed READ adjustedFeed CONSTANT)

    // Na-softening properties
    Q_PROPERTY(double caLeakage READ caLeakage WRITE setCaLeakage NOTIFY caLeakageChanged)
    Q_PROPERTY(double mgLeakage READ mgLeakage WRITE setMgLeakage NOTIFY mgLeakageChanged)

    // ph-adjustment properties
    Q_PROPERTY(double targetPh READ targetPh WRITE setTargetPh NOTIFY targetPhChanged)
    Q_PROPERTY(double acidConcentration READ acidConcentration WRITE setAcidConcentration NOTIFY acidConcentrationChanged)
    Q_PROPERTY(Acid dosingAcid READ dosingAcid WRITE setDosingAcid NOTIFY dosingAcidChanged)

public:
    enum FeedAdjustment { NoAdjusment, NaSoftering, pHAdjustment };
    enum Acid { H2SO4, HCl, NaOH };

    explicit ROScalingElement(ROFlow* const feed = nullptr);

    ROFlow* const feed() const;
    ROFlow* const adjustedFeed() const;
    FeedAdjustment adjustment() const;

    void setFeed(ROFlow* const newFeed);
    void setAdjustment(FeedAdjustment adjustment);

    // Na-softering params
    double caLeakage() const;
    void setCaLeakage(double value); // set
    void updateCaLeakage(double value);
    bool caLeakageExplicit() const { return _CaLeakageExplicit; }

    double mgLeakage() const;
    void setMgLeakage(double value);
    void updateMgLeakage(double value);
    bool mgLeakageExplicit() const { return _MgLeakageExplicit; }

    // Ph-adjustment params
    double targetPh() const;
    void setTargetPh(double value);

    double acidConcentration() const;
    void setAcidConcentration(double value);

    Acid dosingAcid() const;
    void setDosingAcid(Acid value);



signals:
    void feedChanged();
    void adjustmentChanged();

    void caLeakageChanged();
    void mgLeakageChanged();

    void targetPhChanged();
    void acidConcentrationChanged();
    void dosingAcidChanged();
    
private slots:
    void adjust(); // вызывается каждый раз при изменении входных данных

private:
    ROFlow* _feed;
    ROFlow* _adjustedFeed;   
    FeedAdjustment _adjustment;

    double _CaLeakage;
    double _MgLeakage;

    bool _CaLeakageExplicit;
    bool _MgLeakageExplicit;

    double _targetPh;
    double _acidConcentration;
    Acid _dosingAcid;

    bool _calculating; // защита от бесконечной рекурсии




public slots:
    void reset();
};

#endif // ROSCALINGELEMENT_H

