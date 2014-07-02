#include "romembranesize.h"

ROMembraneSize::ROMembraneSize(const double& diameter, const double& length):
    _diameter(diameter), _length(length){}

const double ROMembraneSize::diameter() const { return _diameter; }
const double ROMembraneSize::length() const { return _length; }
const QString ROMembraneSize::toString() const {
    return QString("%1 × %2").arg(QString::number(_diameter, 'f', 2),
                                       QString::number(_length, 'f', 2));
}



ROMembraneSize::ROMembraneSize(const ROMembraneSize& membraneSize) {
    _diameter = membraneSize.diameter();
    _length = membraneSize.length();
}

ROMembraneSize& ROMembraneSize::operator= (const ROMembraneSize& membraneSize) {
    _diameter = membraneSize.diameter();
    _length = membraneSize.length();
    return *this;
}
