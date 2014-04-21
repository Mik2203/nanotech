#include "rofeed.h"

ROFeed::ROFeed(double part, ROFlow* flow, QObject *parent) :
    _flow(flow ? flow : new ROFlow()), _part(part),
    _deleteFlow(flow ? false: true), QObject(parent){}

ROFeed::~ROFeed() { if (_deleteFlow) delete _flow; }

ROFlow* const ROFeed::flow() const { return _flow; }

double ROFeed::part() const { return _part; }
void ROFeed::setPart(double value) { _part = qBound(0.01, value, 100.0); emit partChanged(); }

bool ROFeed::deleteFlow() const { return _deleteFlow; }
void ROFeed::setDeleteFlow(bool needDelete) { _deleteFlow = needDelete; emit deleteFlowChanged(); }

ROFeed* ROFeed::clone() {
    ROFeed* newFeed = new ROFeed(this->part());
    newFeed->copyDataFrom(this); //newFeed->flow()->copyDataFrom(this->flow());
    return newFeed;
}

void ROFeed::copyDataFrom(const ROFeed* const from) {
    this->flow()->copyDataFrom(from->flow());
    this->setPart(from->part());
    this->setDeleteFlow(from->deleteFlow());
}


void ROFeed::reset() {
    setPart(1.0);
    flow()->reset();
}
