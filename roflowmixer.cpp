#include "roflowmixer.h"

ROFlowMixer::ROFlowMixer(unsigned filter):
    _outputFlow(0), _filter(filter) {}

ROFlowMixer::~ROFlowMixer() { }



ROFlow* ROFlowMixer::outputFlow() const { return _outputFlow; }
ROFeed* ROFlowMixer::feed(int feedIndex) const { return _inputFeeds.value(feedIndex); }
ROFeed* ROFlowMixer::addFeed(ROFeed* feed, FlowOperation op) {
    if (_inputFeeds.contains(feed)) return feed;
    _inputFeeds << feed;
    _inputOps << op;

    if (_filter & ROFlowMixer::FlowRate) {
        connect(feed->flow(), SIGNAL(rateChanged()), this, SLOT(mixRate()));
        connect(feed, SIGNAL(partChanged()), this, SLOT(mixRate()));
    }
    if (_filter & ROFlowMixer::FlowTemperature) {
        connect(feed->flow(), SIGNAL(temperatureChanged()), this, SLOT(mixTemperature()));
        connect(feed, SIGNAL(partChanged()), this, SLOT(mixTemperature()));

    }
//    if (_filter & ROFlowMixer::FlowPH) {
//        connect(feed->flow(), SIGNAL(pHChanged()), this, SLOT(mixPH()));
//        connect(feed, SIGNAL(partChanged()), this, SLOT(mixPH()));
//    }
    if (_filter & ROFlowMixer::FlowSolutes) {
        connect(feed->flow(), SIGNAL(solutesChanged()), this, SLOT(mixSolutes()));
        connect(feed, SIGNAL(partChanged()), this, SLOT(mixSolutes()));
    }
    if (_filter & ROFlowMixer::FlowSolutes) {
        connect(feed->flow(), SIGNAL(pressureChanged()), this, SLOT(mixPressure()));
        connect(feed->flow(), SIGNAL(osmoticPressureChanged()), this, SLOT(mixPressure()));
        connect(feed, SIGNAL(partChanged()), this, SLOT(mixPressure()));
    }
    recalculate();
    return feed;
}

ROFeed* ROFlowMixer::addFeed(ROFlow* flow, FlowOperation op, double part) { return addFeed(new ROFeed(part, flow), op); }
bool ROFlowMixer::removeFeed(ROFeed* feed) { return removeFeed(_inputFeeds.indexOf(feed)); }
bool ROFlowMixer::removeFeed(ROFlow* flow) {
    for (int feedIndex = 0; feedIndex < feedCount(); ++feedIndex) {
        if (_inputFeeds[feedIndex]->flow() == flow) {
            return removeFeed(feedIndex);
        }
    }
    return false;
}

bool ROFlowMixer::removeFeed(int feedIndex)  {
    if (0 <= feedIndex && feedIndex < _inputFeeds.count()) {
        ROFeed* removingFeed = _inputFeeds.takeAt(feedIndex);
        _inputOps.removeAt(feedIndex);
        disconnect(removingFeed->flow(), 0, this, 0);
        //delete removingFeed; // auto disconnects partChanged
        recalculate();
        return true;
    }
    return false;
}

void ROFlowMixer::clearFeeds()
{
    while (_inputFeeds.count())
        removeFeed(0);
}

void ROFlowMixer::reset()
{
    clearFeeds();
    setOutputFlow(nullptr);
}
int ROFlowMixer::feedCount() const { return _inputFeeds.count(); }

void ROFlowMixer::mixRate() {
    if (_outputFlow) {

        double newRate = 0.0;

        double totalPart = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) totalPart += feed(feedIdx)->part();

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd:       newRate += feed(feedIdx)->flow()->rate() * feed(feedIdx)->part()/* / totalPart*/; break;
            case FlowSubtract:  newRate -= feed(feedIdx)->flow()->rate() * feed(feedIdx)->part()/* / totalPart*/; break;
            }
        }
        outputFlow()->setRate(newRate);
    }
}

void ROFlowMixer:: mixPressure() {
    if (_outputFlow) {

        double newPressure = 0.0;
        double newOsmPressure = 0.0;
        double totalPart = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) totalPart += feed(feedIdx)->part();

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd: {
                newPressure += feed(feedIdx)->flow()->pressure() * feed(feedIdx)->part();
                newOsmPressure += feed(feedIdx)->flow()->osmoticPressure() * feed(feedIdx)->part();
                break;
            }
            case FlowSubtract: {
                newPressure -= feed(feedIdx)->flow()->pressure() * feed(feedIdx)->part();
                newOsmPressure -= feed(feedIdx)->flow()->osmoticPressure() * feed(feedIdx)->part();
                break;
            }
            }
        }
        outputFlow()->setPressure(newPressure);
        //outputFlow()->updateOsmoticPressure(newOsmPressure);
    }
}

void ROFlowMixer::mixTemperature() {
    if (_outputFlow) {
        double newTemp = 0.0;

        double totalPart = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) totalPart += feed(feedIdx)->part();

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd:       newTemp += feed(feedIdx)->flow()->temperature() * feed(feedIdx)->part() / totalPart; break;
            case FlowSubtract:  newTemp -= feed(feedIdx)->flow()->temperature() * feed(feedIdx)->part() / totalPart; break;
            }
        }
        outputFlow()->setTemperature(newTemp);
    }
}

void ROFlowMixer::mixPH() {
    if (_outputFlow) {
        double newPH = 0.0;

        double totalPart = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) totalPart += feed(feedIdx)->part();

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd:       newPH += feed(feedIdx)->flow()->pH() * feed(feedIdx)->part() / totalPart; break;
            case FlowSubtract:  newPH -= feed(feedIdx)->flow()->pH() * feed(feedIdx)->part() / totalPart; break;
            }
        }
        outputFlow()->setPH(newPH);
    }
}

void ROFlowMixer::mixSolutes() {
    if (_outputFlow) {
        outputFlow()->solutes()->beginChange();
        outputFlow()->solutes()->reset();

        double totalPart = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) totalPart += feed(feedIdx)->part();


        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd: {
                for (int si = 0; si < ROSolutes::TotalIons; ++si) {
                    outputFlow()->solutes()->setMeql(si, outputFlow()->solutes()->meql(si) +
                                                               feed(feedIdx)->flow()->solutes()->meql(si) * feed(feedIdx)->part() / totalPart);
                }
                break;
            }
            case FlowSubtract:  {
                for (int si = 0; si < ROSolutes::TotalIons; ++si) {
                    outputFlow()->solutes()->setMeql(si, outputFlow()->solutes()->meql(si) -
                                                               feed(feedIdx)->flow()->solutes()->meql(si) * feed(feedIdx)->part() / totalPart);
                }
                break;
            }
            }

        }
        outputFlow()->solutes()->endChange();
    }
    mixPH();
}

void ROFlowMixer::setOutputFlow(ROFlow* flow) {
    _outputFlow = flow;

    recalculate();

    Q_EMIT outputFlowChanged();
}

void ROFlowMixer::recalculate() {
    if (!_outputFlow) return;
    if (_filter & ROFlowMixer::FlowRate) {
        mixRate();
    }
    if (_filter & ROFlowMixer::FlowTemperature) {
        mixTemperature();
    }

    if (_filter & ROFlowMixer::FlowSolutes) {
        mixSolutes();
    }
    if (_filter & ROFlowMixer::FlowPressure) {
        mixPressure();
    }
}

