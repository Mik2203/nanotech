#include "roscalingelement.h"
#include "romath.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

ROScalingElement::ROScalingElement(ROFlow* const feed) :
    _feed(nullptr), _adjustedFeed(new ROFlow()),
    _CaLeakage(0.0), _MgLeakage(0.0),
    _CaLeakageExplicit(false), _MgLeakageExplicit(false),
    _calculating(false),
    _adjustment(NoAdjusment),
    _dosingAcid(H2SO4),
    _targetPh(7.6),
    _acidConcentration(1.0){
    setFeed(feed);
}

ROFlow* const ROScalingElement::feed() const { return _feed; }
ROFlow* const ROScalingElement::adjustedFeed() const { return _adjustedFeed; }
ROScalingElement::FeedAdjustment ROScalingElement::adjustment() const { return _adjustment; }

void ROScalingElement::setFeed(ROFlow* const newFeed) {
    if (_feed == newFeed)
        return;

    if (_feed)
        disconnect(_feed, 0, this, 0);

    _feed = newFeed;

    connect(_feed, SIGNAL(solutesChanged()), this, SLOT(adjust()));
    connect(_feed, SIGNAL(pHChanged()), this, SLOT(adjust()));
    connect(_feed, SIGNAL(temperatureChanged()), this, SLOT(adjust()));

    adjust();
    Q_EMIT feedChanged();
}

void ROScalingElement::setAdjustment(ROScalingElement::FeedAdjustment adjustment) {
    if (_adjustment != adjustment) {
        _adjustment = adjustment;
        adjust();
        Q_EMIT adjustmentChanged();
    }
}

double ROScalingElement::caLeakage() const { return _CaLeakage; }

void ROScalingElement::setCaLeakage(double value) {
    _CaLeakageExplicit = true;
    updateCaLeakage(value);
}

double ROScalingElement::mgLeakage() const { return _MgLeakage; }

void ROScalingElement::setMgLeakage(double value) {

    _MgLeakageExplicit = true;
    updateMgLeakage(value);
}

double ROScalingElement::targetPh() const { return _targetPh; }

void ROScalingElement::setTargetPh(double targetPh) {
    targetPh = qBound(1.0, targetPh, 15.0); // TODO CONSTS

    if (adjustment() == pHAdjustment) {
        // при дозировании щелочи (NaOH) pH не может быть меньше исходного значения
        // при дозировании кислоты (H2SO4, HCl) pH не может быть больше исходного значения
        switch (dosingAcid()) {
        case NaOH:
            targetPh = qMax(feed()->pH(), targetPh);
            break;
        case H2SO4:
        case HCl:
            targetPh = qMin(feed()->pH(), targetPh);
            break;
        }
    }

    if (_targetPh != targetPh) {
        _targetPh = targetPh;
        adjust();
        Q_EMIT targetPhChanged();
    }
}

double ROScalingElement::acidConcentration() const { return _acidConcentration; }

void ROScalingElement::setAcidConcentration(double value) {
    value = qBound(0.01, value, 1.0); // TODO
    if (_acidConcentration != value) {
        _acidConcentration = value;
        adjust();
        Q_EMIT acidConcentrationChanged();
    }
}

ROScalingElement::Acid ROScalingElement::dosingAcid() const { return _dosingAcid; }

void ROScalingElement::setDosingAcid(ROScalingElement::Acid value) {
    if (_dosingAcid != value) {
        _dosingAcid = value;
        adjust();
        Q_EMIT dosingAcidChanged();
    }
}



void ROScalingElement::adjust() {
    if (!_feed || _calculating) return;
    _calculating = true;
    // first copy data to adjusted feed
    _adjustedFeed->copyDataFrom(_feed);

    switch (_adjustment) {
    case NoAdjusment: break;
    case NaSoftering: {
        // Ca adjustment

        if (_feed->solutes()->isIonFilled(ROSolutes::Ca)) {
            updateCaLeakage(_CaLeakageExplicit ? _CaLeakage : _feed->solutes()->mgl(ROSolutes::Ca));
            _adjustedFeed->solutes()->setMgl(ROSolutes::Ca, _CaLeakage);
            double diffCa = _feed->solutes()->meql(ROSolutes::Ca) - _adjustedFeed->solutes()->meql(ROSolutes::Ca);
            _adjustedFeed->solutes()->setMeql(ROSolutes::Na, _adjustedFeed->solutes()->meql(ROSolutes::Na) + diffCa);
        }

        // Mg adjustment
        if (_feed->solutes()->isIonFilled(ROSolutes::Mg)) {
            updateMgLeakage(_MgLeakageExplicit ? _MgLeakage : _feed->solutes()->mgl(ROSolutes::Mg));
            _adjustedFeed->solutes()->setMgl(ROSolutes::Mg, _MgLeakage);
            double diffMg = _feed->solutes()->meql(ROSolutes::Mg) - _adjustedFeed->solutes()->meql(ROSolutes::Mg);
            _adjustedFeed->solutes()->setMeql(ROSolutes::Na, _adjustedFeed->solutes()->meql(ROSolutes::Na) + diffMg);
        }
        break;
    }
    case pHAdjustment: {
        setTargetPh(_targetPh);

        // в cPh стоит 3- для H2SO4 и HCl; и -11, а не -14 для NaOH, т.к. сразу включен коэффициент перевода в мэкв.
        switch(_dosingAcid) {
        case H2SO4: {
            double cPh = pow(10.0, 3-_targetPh) - pow(10.0, 3-_feed->pH());
            _adjustedFeed->solutes()->addValue(ROSolutes::SO4, cPh, ROSolutes::Meql);
            break;
        }
        case HCl: {
            double cPh = pow(10.0, 3-_targetPh) - pow(10.0, 3-_feed->pH());
            _adjustedFeed->solutes()->addValue(ROSolutes::Cl, cPh, ROSolutes::Meql);
            break;
        }
        case NaOH: {
            double pOHfeed = 14 - _feed->pH();
            double pOHtarget = 14 - _targetPh;

            double CpOH = pow(10, -pOHtarget) - pow(10, -pOHfeed);

            double HCO3feed = _feed->solutes()->value(ROSolutes::HCO3, ROSolutes::MolL);
            double Hfeed = pow(10, -_feed->pH());

            double HammaH = _feed->solutes()->hamma(ROSolutes::H);
            double HammaHCO3 = _feed->solutes()->hamma(ROSolutes::HCO3);
            double HammaCO3 = _feed->solutes()->hamma(ROSolutes::CO3);

            double t = _feed->temperature();
            double k1_ = k1(t);
            double k2_ = k2(t);

            double CO2feed = HCO3feed * HammaHCO3 *
                    Hfeed * HammaH /
                    k1_;

            double Htarget = pow(10, -_targetPh);

            double g10 = Htarget * Htarget * HammaH * HammaH + k1_ * Htarget * HammaH + k1_ * k2_;
            double g11 = 1 / g10;


            double CO3feed = k2_ * HCO3feed * HammaHCO3 /
                    (Hfeed * HammaH * HammaCO3);
            double C_total = HCO3feed * HammaHCO3 + CO3feed * HammaCO3 + CO2feed;

            double CO2target = Htarget * Htarget * HammaH * HammaH * g11 * C_total;
            double k5 = CO2feed - CO2target;

            double Na_delta = CpOH + k5;
            double HCO3_delta = k1_ * g11 * HammaH * Htarget * C_total / HammaHCO3 - HCO3feed;

//            qDebug() << "Na =" << Na_delta;
//            qDebug() << "HCO3 =" << HCO3_delta;

            _adjustedFeed->solutes()->addValue(ROSolutes::Na, Na_delta / acidConcentration(), ROSolutes::MolL);
            _adjustedFeed->solutes()->addValue(ROSolutes::HCO3, HCO3_delta / acidConcentration(), ROSolutes::MolL);

            break;
        }
        }
        _adjustedFeed->setPH(_targetPh);

        break;
    }
    }
    _calculating = false;
}

void ROScalingElement::updateCaLeakage(double value) {
    _CaLeakage = value;
    if (_feed) _CaLeakage = qBound(0.0, _CaLeakage, _feed->solutes()->mgl(ROSolutes::Ca));
    adjust();
    Q_EMIT caLeakageChanged();
}

void ROScalingElement::updateMgLeakage(double value) {
    _MgLeakage = value;
    if (_feed) _MgLeakage = qBound(0.0, _MgLeakage, _feed->solutes()->mgl(ROSolutes::Mg));
    adjust();
    Q_EMIT mgLeakageChanged();
}

void ROScalingElement::reset() {
    setAdjustment(NoAdjusment);
    _CaLeakage = 0.0;
    _CaLeakageExplicit = false;
    _MgLeakage = 0.0;
    _MgLeakageExplicit = false;
    _targetPh = 7.6; // TODO const
    _acidConcentration = 1.0; // TODO const
    _dosingAcid = H2SO4;
}
