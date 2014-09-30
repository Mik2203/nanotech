#include "roflowmixer.h"

ROFlowMixer::ROFlowMixer(unsigned filter):
    _outputFlow(0), _filter(filter) {}

ROFlowMixer::~ROFlowMixer() {

}



ROFlow* ROFlowMixer::outputFlow() const { return _outputFlow; }
ROFeed* ROFlowMixer::feed(int feedIndex) const { return _inputFeeds.value(feedIndex); }
ROFeed* ROFlowMixer::addFeed(ROFeed* feed, FlowOperation op) {
    if (_inputFeeds.contains(feed)) return feed;
    _inputFeeds << feed;
    _inputOps << op;

    // все параметры зависят от объема потока
    connect(feed->flow(), SIGNAL(rateChanged()), this, SLOT(recalculate()));
    connect(feed, SIGNAL(partChanged()), this, SLOT(recalculate()));
    if (_filter & ROFlowMixer::FlowTemperature) {
        connect(feed->flow(), SIGNAL(temperatureChanged()), this, SLOT(mixTemperature()));
        connect(feed, SIGNAL(partChanged()), this, SLOT(mixTemperature()));
    }
    if (_filter & ROFlowMixer::FlowSolutes) {
        connect(feed->flow(), SIGNAL(solutesChanged()), this, SLOT(mixSolutes()));
        connect(feed, SIGNAL(partChanged()), this, SLOT(mixSolutes()));
    }
    if (_filter & ROFlowMixer::FlowPressure) {
        connect(feed->flow(), SIGNAL(pressureChanged()), this, SLOT(mixPressure()));
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
        if (removingFeed->flow())
            disconnect(removingFeed->flow(), 0, this, 0);
        //delete removingFeed; // auto disconnects partChanged
        recalculate();
        return true;
    }
    return false;
}

//void ROFlowMixer::removeDeadFeed(int feedIndex)
//{
//    _inputFeeds.removeAt(feedIndex);
//    _inputOps.removeAt(feedIndex);
//    recalculate();
//}

void ROFlowMixer::clearFeeds()
{
    while (_inputFeeds.count()) {
        _inputOps.removeAt(0);
        ROFeed* removingFeed = _inputFeeds.takeAt(0);
        if (removingFeed->flow()) {
             disconnect(removingFeed->flow(), 0, this, 0);
        }
    }
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
        double totalPart = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) totalPart += feed(feedIdx)->part();

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd: {
                newPressure += feed(feedIdx)->flow()->pressure() * feed(feedIdx)->part();
                break;
            }
            case FlowSubtract: {
                newPressure -= feed(feedIdx)->flow()->pressure() * feed(feedIdx)->part();
                break;
            }
            }
        }
        outputFlow()->setPressure(newPressure);
    }
}

void ROFlowMixer::mixTemperature() {
    if (_outputFlow) {
        double newTemp = 0.0;

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            double feedCoeff = _coeff(feedIdx);
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd:       newTemp += feed(feedIdx)->flow()->temperature() * feedCoeff; break;
            case FlowSubtract:  newTemp -= feed(feedIdx)->flow()->temperature() * feedCoeff; break;
            }
        }
        outputFlow()->setTemperature(newTemp);
    }
}

void ROFlowMixer::mixPH() {
    if (_outputFlow) {
        double newPH = 0.0;

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            double feedCoeff = _coeff(feedIdx);
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd:       newPH += feed(feedIdx)->flow()->pH() * feedCoeff; break;
            case FlowSubtract:  newPH -= feed(feedIdx)->flow()->pH() * feedCoeff; break;
            }
        }
        outputFlow()->setPH(newPH);
    }
}

void ROFlowMixer::mixSolutes() {
    if (_outputFlow) {
        outputFlow()->solutes()->beginChange();
        outputFlow()->solutes()->reset();


        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            double feedCoeff = _coeff(feedIdx);
            switch(_inputOps.at(feedIdx)) {
            case FlowAdd: {
                for (int si = 0; si < ROSolutes::TotalIons; ++si) {
                    outputFlow()->solutes()->setMeql(si, outputFlow()->solutes()->meql(si) +
                                                               feed(feedIdx)->flow()->solutes()->meql(si) * feedCoeff);
                }
                break;
            }
            case FlowSubtract:  {
                for (int si = 0; si < ROSolutes::TotalIons; ++si) {
                    outputFlow()->solutes()->setMeql(si, outputFlow()->solutes()->meql(si) -
                                                               feed(feedIdx)->flow()->solutes()->meql(si) * feedCoeff);
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

double ROFlowMixer::_coeff(int feedIndex)
{
    return (_partCoeff(feedIndex)) / _totalCoeff();
}

double ROFlowMixer::_partCoeff(int feedIndex)
{
    return feed(feedIndex)->part() * (feed(feedIndex)->flow()->rate() != 0.0 ? feed(feedIndex)->flow()->rate(): 1.0);
}

double ROFlowMixer::_totalCoeff()
{
    double totalCoeff = 0.0;
    for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx)
        totalCoeff += _partCoeff(feedIdx);
    return totalCoeff;
}

