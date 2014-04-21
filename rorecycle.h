#ifndef RORECYCLE_H
#define RORECYCLE_H
#include "rodestinationelement.h"

class RORecycle
{
public:
    RORecycle();
    RODestinationElement* destination();
    double part();
private:
    RODestinationElement* _destination;
    double _part;
};

#endif // RORECYCLE_H
