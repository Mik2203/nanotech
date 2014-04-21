#ifndef ROFEED_H
#define ROFEED_H

#include <QObject>

#include "roflow.h"

// содержит информацию о входных потоках: доли потока и ионах.

class ROFeed : public QObject {
    Q_OBJECT
    Q_PROPERTY(double part READ part WRITE setPart NOTIFY partChanged)
    Q_PROPERTY(bool deleteFlow READ deleteFlow WRITE setDeleteFlow NOTIFY deleteFlowChanged)
    Q_PROPERTY(ROFlow* flow READ flow CONSTANT)

public:
    explicit ROFeed(double part = 1.0, ROFlow* flow = 0, QObject *parent = 0);
    ~ROFeed();
    
    ROFlow* const flow() const;

    double part() const;
    void setPart(double value);

    // флаг удаления потока ROFlow при удалении ROFeed
    bool deleteFlow() const;
    void setDeleteFlow(bool needDelete);

    ROFeed* clone();
    void copyDataFrom(const ROFeed* const);

signals:
    void partChanged();
    void deleteFlowChanged();
public slots:
    void reset();
    
private:
    ROFlow* const _flow;
    double _part;
    bool _deleteFlow;
};

#endif // ROFEED_H
