#include "qwheelarea.h"
#include <QGraphicsSceneWheelEvent>

QWheelArea::QWheelArea(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_horizontalMinimumValue(0),
      m_horizontalMaximumValue(0),
      m_verticalMinimumValue(0),
      m_verticalMaximumValue(0),
      m_horizontalValue(0),
      m_verticalValue(0),
      m_verticalDelta(0),
      m_horizontalDelta(0),
      m_scrollSpeed(1.0)
{

}

QWheelArea::~QWheelArea()
{

}

bool QWheelArea::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::GraphicsSceneWheel: {
        QGraphicsSceneWheelEvent *we = reinterpret_cast<QGraphicsSceneWheelEvent *>(e);

        if (we->orientation() == Qt::Vertical)
            setVerticalDelta(we->delta());
        else
            setHorizontalDelta(we->delta());

        return true;
    }
    case QEvent::Wheel: {
        QWheelEvent *we = static_cast<QWheelEvent *>(e);

        if (we->orientation() == Qt::Vertical)
            setVerticalDelta(we->delta());
        else
            setHorizontalDelta(we->delta());

        return true;
    }
    default:
        break;
    }

    return QDeclarativeItem::event(e);
}

void QWheelArea::setHorizontalMinimumValue(qreal value)
{
    m_horizontalMinimumValue = value;
}

qreal QWheelArea::horizontalMinimumValue() const
{
    return m_horizontalMinimumValue;
}

void QWheelArea::setHorizontalMaximumValue(qreal value)
{
    m_horizontalMaximumValue = value;
}

qreal QWheelArea::horizontalMaximumValue() const
{
    return m_horizontalMaximumValue;
}

void QWheelArea::setVerticalMinimumValue(qreal value)
{
    m_verticalMinimumValue = value;
}

qreal QWheelArea::verticalMinimumValue() const
{
    return m_verticalMinimumValue;
}

void QWheelArea::setVerticalMaximumValue(qreal value)
{
    m_verticalMaximumValue = value;
}

qreal QWheelArea::verticalMaximumValue() const
{
    return m_verticalMaximumValue;
}

void QWheelArea::setHorizontalValue(qreal value)
{
    value = qBound<qreal>(m_horizontalMinimumValue, value, m_horizontalMaximumValue);

    if (value != m_horizontalValue) {
        m_horizontalValue = value;
        emit horizontalValueChanged();
    }
}

qreal QWheelArea::horizontalValue() const
{
    return m_horizontalValue;
}

void QWheelArea::setVerticalValue(qreal value)
{
    value = qBound<qreal>(m_verticalMinimumValue, value, m_verticalMaximumValue);

    if (value != m_verticalValue) {
        m_verticalValue = value;
        emit verticalValueChanged();
    }
}

qreal QWheelArea::verticalValue() const
{
    return m_verticalValue;
}

void QWheelArea::setVerticalDelta(qreal value)
{
    m_verticalDelta = m_scrollSpeed * value / 15;
    setVerticalValue(m_verticalValue - m_verticalDelta);

    emit verticalWheelMoved();
}

qreal QWheelArea::verticalDelta() const
{
    return m_verticalDelta;
}

void QWheelArea::setHorizontalDelta(qreal value)
{
    m_horizontalDelta = value / 15;
    setHorizontalValue(m_horizontalValue - m_horizontalDelta);

    emit horizontalWheelMoved();
}

qreal QWheelArea::horizontalDelta() const
{
    return m_horizontalDelta;
}

void QWheelArea::setScrollSpeed(qreal value)
{
    if (value != m_scrollSpeed) {
        m_scrollSpeed = value;
        emit scrollSpeedChanged();
    }
}

qreal QWheelArea::scrollSpeed() const
{
    return m_scrollSpeed;
}
