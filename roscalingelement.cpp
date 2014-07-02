#include "roscalingelement.h"
#include <QDebug>

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

void ROScalingElement::setFeed(ROFlow* newFeed) {
    if (_feed != newFeed) {
        if (_feed) disconnect(_feed, 0, this, 0);

        _feed = newFeed;

        connect(_feed, SIGNAL(solutesChanged()), this, SLOT(adjust()));
        connect(_feed, SIGNAL(pHChanged()), this, SLOT(adjust()));
        connect(_feed, SIGNAL(temperatureChanged()), this, SLOT(adjust()));

        adjust();
        Q_EMIT feedChanged();
    }
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

void ROScalingElement::setTargetPh(double value) {
    value = qBound(1.0, value, 15.0); // TODO
    if (_targetPh != value) {
        _targetPh = value;
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
        double c = pow(10.0, -_targetPh) - pow(10.0, -_feed->pH());
        // TODO ограничения за выход в минус
        switch(_dosingAcid) {
        case H2SO4: {
            double so4MolarMass = 32.06 + 15.9994*4;
            double h2so4MolarMass = 2*1.0079 + so4MolarMass;
            double so4Part = so4MolarMass / h2so4MolarMass;
            double v = h2so4MolarMass * c * 1000.0 / 2 / acidConcentration();
            double so4Add = v * so4Part;
            _adjustedFeed->solutes()->setMgl(ROSolutes::SO4, _adjustedFeed->solutes()->mgl(ROSolutes::SO4) + so4Add);
            break;
        }
        case HCl: {
            double clMolarMass = 35.4527;
            double hclMolarMass = 1.0079 + clMolarMass;
            double clPart = clMolarMass / hclMolarMass;
            double v = hclMolarMass * c * 1000.0 / acidConcentration();
            double clAdd = v * clPart;
            _adjustedFeed->solutes()->setMgl(ROSolutes::Cl, _adjustedFeed->solutes()->mgl(ROSolutes::Cl) + clAdd);
            break;
        }
        case NaOH: {
            double naMolarMass = 22.98977;
            double naohMolarMass = 15.9994 + 1.0079 + naMolarMass;
            double naPart = naMolarMass / naohMolarMass;
            double v = naohMolarMass * c * 1000.0 / acidConcentration();
            double naAdd = v * naPart;
            _adjustedFeed->solutes()->setMgl(ROSolutes::Na, _adjustedFeed->solutes()->mgl(ROSolutes::Na) + naAdd);
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
