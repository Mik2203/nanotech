#include <QEvent>
#include <QApplication>
#include <QGraphicsSceneEvent>
#include <QDebug>

#ifndef QT_NO_ACCESSIBILITY
#include <QAccessible>
#endif

#include "qrangemodel.h"
#include "qrangemodel_p.h"

QRangeModelPrivate::QRangeModelPrivate(QRangeModel *qq)
    : q_ptr(qq)
{
}

QRangeModelPrivate::~QRangeModelPrivate()
{
}

void QRangeModelPrivate::init()
{
    minimum = 0;
    maximum = 99;
    stepSize = 0;
    value = 0;
    pos = 0;
    posatmin = 0;
    posatmax = 0;
    inverted = false;
}

qreal QRangeModelPrivate::publicPosition(qreal position) const
{
    // Calculate the equivalent stepSize for the position property.
    const qreal min = effectivePosAtMin();
    const qreal max = effectivePosAtMax();
    const qreal valueRange = maximum - minimum;
    const qreal positionValueRatio = valueRange ? (max - min) / valueRange : 0;
    const qreal positionStep = stepSize * positionValueRatio;

    if (positionStep == 0)
        return (min < max) ? qBound(min, position, max) : qBound(max, position, min);

    const int stepSizeMultiplier = (position - min) / positionStep;

    // Test whether value is below minimum range
    if (stepSizeMultiplier < 0)
        return min;

    qreal leftEdge = (stepSizeMultiplier * positionStep) + min;
    qreal rightEdge = ((stepSizeMultiplier + 1) * positionStep) + min;

    if (min < max) {
        leftEdge = qMin(leftEdge, max);
        rightEdge = qMin(rightEdge, max);
    } else {
        leftEdge = qMax(leftEdge, max);
        rightEdge = qMax(rightEdge, max);
    }

    if (qAbs(leftEdge - position) <= qAbs(rightEdge - position))
        return leftEdge;
    return rightEdge;
}

qreal QRangeModelPrivate::publicValue(qreal value) const
{
    // It is important to do value-within-range check this
    // late (as opposed to during setPosition()). The reason is
    // QML bindings; a position that is initially invalid because it lays
    // outside the range, might become valid later if the range changes.

    if (stepSize == 0)
        return qBound(minimum, value, maximum);

    const int stepSizeMultiplier = (value - minimum) / stepSize;

    // Test whether value is below minimum range
    if (stepSizeMultiplier < 0)
        return minimum;

    const qreal leftEdge = qMin(maximum, (stepSizeMultiplier * stepSize) + minimum);
    const qreal rightEdge = qMin(maximum, ((stepSizeMultiplier + 1) * stepSize) + minimum);
    const qreal middle = (leftEdge + rightEdge) / 2;

    return (value <= middle) ? leftEdge : rightEdge;
}

void QRangeModelPrivate::emitValueAndPositionIfChanged(const qreal oldValue, const qreal oldPosition)
{
    Q_Q(QRangeModel);

    // Effective value and position might have changed even in cases when e.g. d->value is
    // unchanged. This will be the case when operating with values outside range:
    const qreal newValue = q->value();
    const qreal newPosition = q->position();
    if (!qFuzzyCompare(newValue, oldValue))
        emit q->valueChanged(newValue);
    if (!qFuzzyCompare(newPosition, oldPosition))
        emit q->positionChanged(newPosition);
}

QRangeModel::QRangeModel(QObject *parent)
    : QObject(parent), d_ptr(new QRangeModelPrivate(this))
{
    Q_D(QRangeModel);
    d->init();
}

QRangeModel::QRangeModel(QRangeModelPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(QRangeModel);
    d->init();
}


QRangeModel::~QRangeModel()
{
    delete d_ptr;
    d_ptr = nullptr;
}


void QRangeModel::setPositionRange(qreal min, qreal max)
{
    Q_D(QRangeModel);

    bool emitPosAtMinChanged = !qFuzzyCompare(min, d->posatmin);
    bool emitPosAtMaxChanged = !qFuzzyCompare(max, d->posatmax);

    if (!(emitPosAtMinChanged || emitPosAtMaxChanged))
        return;

    const qreal oldPosition = position();
    d->posatmin = min;
    d->posatmax = max;

    // When a new positionRange is defined, the position property must be updated based on the value property.
    // For instance, imagine that you have a valueRange of [0,100] and a position range of [20,100],
    // if a user set the value to 50, the position would be 60. If this positionRange is updated to [0,100], then
    // the new position, based on the value (50), will be 50.
    // If the newPosition is different than the old one, it must be updated, in order to emit
    // the positionChanged signal.
    d->pos = d->equivalentPosition(d->value);

    if (emitPosAtMinChanged)
        emit positionAtMinimumChanged(d->posatmin);
    if (emitPosAtMaxChanged)
        emit positionAtMaximumChanged(d->posatmax);

    d->emitValueAndPositionIfChanged(value(), oldPosition);
}

void QRangeModel::setRange(qreal min, qreal max)
{
    Q_D(QRangeModel);

    bool emitMinimumChanged = !qFuzzyCompare(min, d->minimum);
    bool emitMaximumChanged = !qFuzzyCompare(max, d->maximum);

    if (!(emitMinimumChanged || emitMaximumChanged))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();

    d->minimum = min;
    d->maximum = qMax(min, max);

    // Update internal position if it was changed. It can occurs if internal value changes, due to range update
    d->pos = d->equivalentPosition(d->value);

    if (emitMinimumChanged)
        emit minimumChanged(d->minimum);
    if (emitMaximumChanged)
        emit maximumChanged(d->maximum);

    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setMinimum(qreal min)
{
    Q_D(const QRangeModel);
    setRange(min, d->maximum);
}

qreal QRangeModel::minimum() const
{
    Q_D(const QRangeModel);
    return d->minimum;
}

void QRangeModel::setMaximum(qreal max)
{
    Q_D(const QRangeModel);
    // if the new maximum value is smaller than
    // minimum, update minimum too
    setRange(qMin(d->minimum, max), max);
}

qreal QRangeModel::maximum() const
{
    Q_D(const QRangeModel);
    return d->maximum;
}

void QRangeModel::setStepSize(qreal stepSize)
{
    Q_D(QRangeModel);

    stepSize = qMax(qreal(0.0), stepSize);
    if (qFuzzyCompare(stepSize, d->stepSize))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();
    d->stepSize = stepSize;

    emit stepSizeChanged(d->stepSize);
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

qreal QRangeModel::stepSize() const
{
    Q_D(const QRangeModel);
    return d->stepSize;
}

qreal QRangeModel::positionForValue(qreal value) const
{
    Q_D(const QRangeModel);

    const qreal unconstrainedPosition = d->equivalentPosition(value);
    return d->publicPosition(unconstrainedPosition);
}

qreal QRangeModel::position() const
{
    Q_D(const QRangeModel);

    // Return the internal position but observe boundaries and
    // stepSize restrictions.
    return d->publicPosition(d->pos);
}

void QRangeModel::setPosition(qreal newPosition)
{
    Q_D(QRangeModel);

    if (qFuzzyCompare(newPosition, d->pos))
        return;

    const qreal oldPosition = position();
    const qreal oldValue = value();

    // Update position and calculate new value
    d->pos = newPosition;
    d->value = d->equivalentValue(d->pos);
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setPositionAtMinimum(qreal min)
{
    Q_D(QRangeModel);
    setPositionRange(min, d->posatmax);
}

qreal QRangeModel::positionAtMinimum() const
{
    Q_D(const QRangeModel);
    return d->posatmin;
}

void QRangeModel::setPositionAtMaximum(qreal max)
{
    Q_D(QRangeModel);
    setPositionRange(d->posatmin, max);
}

qreal QRangeModel::positionAtMaximum() const
{
    Q_D(const QRangeModel);
    return d->posatmax;
}

qreal QRangeModel::valueForPosition(qreal position) const
{
    Q_D(const QRangeModel);

    const qreal unconstrainedValue = d->equivalentValue(position);
    return d->publicValue(unconstrainedValue);
}

qreal QRangeModel::value() const
{
    Q_D(const QRangeModel);

    // Return internal value but observe boundaries and
    // stepSize restrictions
    return d->publicValue(d->value);
}

void QRangeModel::setValue(qreal newValue)
{
    Q_D(QRangeModel);

    if (qFuzzyCompare(newValue, d->value))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();

    // Update relative value and position
    d->value = newValue;
    d->pos = d->equivalentPosition(d->value);
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setInverted(bool inverted)
{
    Q_D(QRangeModel);
    if (inverted == d->inverted)
        return;

    d->inverted = inverted;
    emit invertedChanged(d->inverted);

    // After updating the internal value, the position property can change.
    setPosition(d->equivalentPosition(d->value));
}

bool QRangeModel::inverted() const
{
    Q_D(const QRangeModel);
    return d->inverted;
}

void QRangeModel::toMinimum()
{
    Q_D(const QRangeModel);
    setValue(d->minimum);
}

void QRangeModel::toMaximum()
{
    Q_D(const QRangeModel);
    setValue(d->maximum);
}
