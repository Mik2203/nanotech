#include "roabstractelement.h"
#include "romath.h"

double ROAbstractElement::recovery() const
{
    if (!permeate() || !feed() || feed()->rate() == 0.0) {
        return 0.0;
    }
    return permeate()->rate() / feed()->rate();
}

double ROAbstractElement::averageFlux() const {
    double totalArea = activeArea();
    return totalArea > 0.0 && permeate() ? (permeate()->rate() * m3ToL) / activeArea() : totalArea;
}

