#ifndef ROMIXERELEMENT_H
#define ROMIXERELEMENT_H

#include <QList>

#include "roflow.h"
#include "rofeed.h"


/* класс для операций над потоками, такими как смешение и разделение.
 */

class ROFlowMixer : public QObject {

    Q_OBJECT

    Q_PROPERTY(ROFlow* outputFlow READ outputFlow WRITE setOutputFlow NOTIFY outputFlowChanged)

public:
    enum FlowOperation { FlowAdd, FlowSubtract };
    enum FlowData {
        //FlowPH = 0x1,
        FlowTemperature = 0x2,
        FlowSolutes = 0x4,
        FlowRate = 0x8,
        FlowPressure = 0x10
    };

    ROFlowMixer(unsigned filter);
    ~ROFlowMixer();

    ROFlow* outputFlow() const;
    ROFeed* feed(int feedFlowIndex) const;

    ROFeed* addFeed(ROFeed* feed, FlowOperation op);
    ROFeed* addFeed(ROFlow* flow, FlowOperation op, double part = 1.0);

    bool removeFeed(ROFeed* feed);
    bool removeFeed(ROFlow* flow);
    bool removeFeed(int feedIndex);

    int feedCount() const;
    void setOutputFlow(ROFlow* flow);

    void recalculate();


private:
    QList<ROFeed*> _inputFeeds;
    QList<FlowOperation> _inputOps;
    ROFlow* _outputFlow;
    unsigned _filter;

private slots:
    void mixRate();
    void mixTemperature();
    void mixPH();
    void mixSolutes();
    void mixPressure();

signals:
    void outputFlowChanged();
};

#endif // ROMIXERELEMENT_H
