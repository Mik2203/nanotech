#include "roabstractelement.h"
#include "romath.h"

double ROAbstractElement::averageFlux() const {
    double totalArea = activeArea();
    return totalArea > 0.0 && permeate() ? (permeate()->rate() * m3ToL) / activeArea() : totalArea;
}
