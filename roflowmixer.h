#ifndef ROMIXERELEMENT_H
#define ROMIXERELEMENT_H

#include <QList>

#include "roflow.h"


/* класс для операций над потоками, такими как смешение и разделение.
 */

class ROFlowMixer : public QObject {

    Q_OBJECT

    Q_PROPERTY(ROFlow* outputFlow READ outputFlow WRITE setOutputFlow NOTIFY outputFlowChanged)

public:
    enum FlowOperation { FlowAdd, FlowSubtract };
    enum FlowData {
        //FlowPH = 0x1,
//        FlowTemperature = 0x2,
        FlowSolutesTemperature = 0x4,
        FlowRate = 0x8,
        FlowPressure = 0x10
    };

    ROFlowMixer(unsigned filter); // TODO typedef
    ~ROFlowMixer();

    ROFlow* outputFlow() const;
    ROFlow* feed(int feedIndex) const;

    ROFlow* addFeed(ROFlow* feed, FlowOperation op);

    bool removeFeed(ROFlow* flow);
    bool removeFeed(int feedIndex);
//    void removeDeadFeed(int feedIndex);
    void clearFeeds();

    void reset();

    int feedCount() const;
    void setOutputFlow(ROFlow* flow);

public slots:
    void recalculate();


private:
    QList<ROFlow *> _inputFeeds;
    QList<FlowOperation> _inputOps;
    ROFlow* _outputFlow;
    unsigned _filter;

    double _coeff(int feedIndex);
    double _totalCoeff();

private slots:
    void mixRate();
//    void mixTemperature();
//    void mixPH();
    void mixSolutesTemperature();
    void mixPressure();

signals:
    void outputFlowChanged();
};

#endif // ROMIXERELEMENT_H
