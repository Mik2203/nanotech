#include "roflowmixer.h"

#include "romath.h"

ROFlowMixer::ROFlowMixer(unsigned filter):
    _outputFlow(0), _filter(filter) {}

ROFlowMixer::~ROFlowMixer() {

}



ROFlow * ROFlowMixer::outputFlow() const { return _outputFlow; }
ROFlow * ROFlowMixer::feed(int feedIndex) const { return _inputFeeds.value(feedIndex); }
ROFlow * ROFlowMixer::addFeed(ROFlow* feed, FlowOperation op) {
    if (_inputFeeds.contains(feed)) return feed;
    _inputFeeds << feed;
    _inputOps << op;

    // все параметры зависят от объема потока
    connect(feed, SIGNAL(rateChanged()), this, SLOT(recalculate()));
    if (_filter & ROFlowMixer::FlowSolutesTemperature) {
        connect(feed, SIGNAL(temperatureChanged()), this, SLOT(mixSolutesTemperature()));
        connect(feed, SIGNAL(solutesChanged()), this, SLOT(mixSolutesTemperature()));
    }
    if (_filter & ROFlowMixer::FlowPressure) {
        connect(feed, SIGNAL(pressureChanged()), this, SLOT(mixPressure()));
    }
    recalculate();
    return feed;
}

bool ROFlowMixer::removeFeed(ROFlow* feed) { return removeFeed(_inputFeeds.indexOf(feed)); }


bool ROFlowMixer::removeFeed(int feedIndex)  {
    if (0 <= feedIndex && feedIndex < _inputFeeds.count()) {
        ROFlow * removingFeed = _inputFeeds.takeAt(feedIndex);
        _inputOps.removeAt(feedIndex);
        if (removingFeed)
            disconnect(removingFeed, 0, this, 0);
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
        ROFlow* removingFeed = _inputFeeds.takeAt(0);
        if (removingFeed) {
             disconnect(removingFeed, 0, this, 0);
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
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            double k = _inputOps.at(feedIdx) == FlowAdd ? 1 : -1;

            newRate += k * feed(feedIdx)->rate();
        }
        outputFlow()->setRate(newRate);
    }
}

void ROFlowMixer:: mixPressure() {
    if (_outputFlow) {

        double newPressure = 0.0;

        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            double k = _inputOps.at(feedIdx) == FlowAdd ? 1 : -1;
            newPressure += k * feed(feedIdx)->pressure();
        }
        outputFlow()->setPressure(newPressure);
    }
}


void ROFlowMixer::mixSolutesTemperature() {
    if (_outputFlow && feedCount()) {
        outputFlow()->solutes()->beginChange();
        outputFlow()->solutes()->reset();

        double newTemp = 0.0;
        double newH = 0.0;
        for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx) {
            double feedCoeff = _coeff(feedIdx);
            double k = _inputOps.at(feedIdx) == FlowAdd ? 1 : -1;

            for (int si = 0; si < ROSolutes::TotalIons; ++si) {
                outputFlow()->solutes()->addValue(si, k * feed(feedIdx)->solutes()->meql(si) * feedCoeff, ROSolutes::Meql);
            }
            newTemp += k * feed(feedIdx)->temperature() * feedCoeff;
            newH += k * feed(feedIdx)->solutes()->meql(ROSolutes::H) * feedCoeff;

        }
        outputFlow()->setTemperature(newTemp);
        outputFlow()->solutes()->setMeql(ROSolutes::H, newH);
        outputFlow()->solutes()->endChange();
    }
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
//    if (_filter & ROFlowMixer::FlowTemperature) {
//        mixTemperature();
//    }
    if (_filter & ROFlowMixer::FlowSolutesTemperature) {
        mixSolutesTemperature();
    }
    if (_filter & ROFlowMixer::FlowPressure) {
        mixPressure();
    }
}

double ROFlowMixer::_coeff(int feedIndex)
{
    return _totalCoeff() != 0.0 ? (feed(feedIndex)->part()) / _totalCoeff() : 0.0;
}

double ROFlowMixer::_totalCoeff()
{
    double totalCoeff = 0.0;
    for (int feedIdx = 0; feedIdx < feedCount(); ++feedIdx)
        totalCoeff += feed(feedIdx)->part();
    return totalCoeff;
}

