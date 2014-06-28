#include "qmlline.h"

#include <QTransform>

#define _USE_MATH_DEFINES // for MSVC 2010 C++
#include "math.h"
#include <QDebug>

double len(const QPointF& p) {
    return sqrt(p.x()*p.x() + p.y()*p.y());
}

//const double qmlLine::_arrowSize = 9.0;

qmlLine::qmlLine(QDeclarativeItem *parent) :
        QDeclarativeItem(parent),
        _color(Qt::black), _penWidth(1), _lineStartType(LineEndNone), _lineEndType(LineEndNone)
{
    // Important, otherwise the paint method is never called
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

void qmlLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen(_color, _penWidth);
    painter->setPen(pen);

    if(smooth() == true) {
        painter->setRenderHint(QPainter::Antialiasing, true);
    }

    // if end types are specified, we need to shorten line
    QPointF initStart = _points.first();
    QPointF initEnd = _points.last();
    truncateEnds();

    QPointF penDelta = QPointF(penWidth() / 2, penWidth() / 2);

    QVector<QPointF> drawPoints = _points;
    for (int i=0; i<drawPoints.count(); i++) {
        drawPoints[i] += penDelta;
    }

    // draw
    painter->drawPolyline(drawPoints.data(), drawPoints.count());

    // restore values
    _points[0] = initStart;
    _points[_points.count()-1] = initEnd;

    drawPoints[0] = initStart + penDelta;
    drawPoints[drawPoints.count()-1] = initEnd + penDelta;


    // Draw line ends
    if (_points.count() >= 2) {
        switch (_lineStartType) {
        case LineEndArrow: drawArrow(painter, drawPoints[0], drawPoints[1]);
        }
        switch (_lineEndType) {
        case LineEndArrow: drawArrow(painter, drawPoints[drawPoints.count()-1], drawPoints[drawPoints.count()-2]);
        }
    }
}

QColor qmlLine::color() const { return _color; }
qmlLine::LineEndType qmlLine::lineStartType() const { return _lineStartType; }
qmlLine::LineEndType qmlLine::lineEndType() const { return _lineEndType; }
int qmlLine::penWidth() const { return _penWidth; }


void qmlLine::setColor(const QColor &color) {
    if(_color == color) return;
    _color = color;
    Q_EMIT colorChanged();
    update();
}

void qmlLine::setLineStartType(qmlLine::LineEndType type) {
    if (_lineStartType != type) {
        _lineStartType = type;
        Q_EMIT lineStartTypeChanged();
    }
}

void qmlLine::setLineEndType(qmlLine::LineEndType type) {
    if (_lineEndType != type) {
        _lineEndType = type;
        Q_EMIT lineEndTypeChanged();
    }
}

void qmlLine::setPenWidth(int newWidth) {
    if(_penWidth == newWidth) return;
    _penWidth = newWidth;
    _arrowSize = _penWidth * 3;
    _arrowPoints[0] = QPointF(0.0, -_arrowSize/2);
    _arrowPoints[1] = QPointF(0.0, _arrowSize/2);
    _arrowPoints[2] = QPointF(_arrowSize, 0.0);
    Q_EMIT penWidthChanged();
    update();
}


void qmlLine::update() {
    qreal minX = 0.0;
    qreal maxX = 0.0;
    qreal minY = 0.0;
    qreal maxY = 0.0;

    if (!_points.isEmpty()) {
        minX = maxX = _points[0].x();
        minY = maxY = _points[0].y();
    }
    Q_FOREACH(const QPointF& p, _points) {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());
        minY = qMin(minY, p.y());
        maxY = qMax(maxY, p.y());
    }

//    setX(minX - _penWidth/2);
//    setY(minY - _penWidth/2);
    _minX = minX;
    _minY = minY;
    setWidth(qAbs(minX - maxX) + _penWidth);
    setHeight(qAbs(minY - maxY) + _penWidth);
}

void qmlLine::drawArrow(QPainter *painter, const QPointF& p0, const QPointF& p1) {
    QBrush fillBrush(_color);
    painter->setPen(Qt::transparent);
    painter->setBrush(fillBrush);

    // FIND ANGLE
    double angle = 0.0;
    QPointF newDirection = p0 - p1;
    angle = acos(newDirection.x() / len(newDirection)); //in radians
    angle = angle * 180.0 / M_PI; //convert to degrees and reverse (because of qt-clockwise rotation)
    if (newDirection.y() < 0.0) angle = -angle;

    QTransform initTrans = painter->worldTransform();
    QPointF p0mapped = initTrans.map(p0);


    QTransform arrowTrans = QTransform::fromTranslate(p0mapped.x(), p0mapped.y());
    arrowTrans.rotate(angle);
    arrowTrans.translate(-_arrowSize * 0.8 /*+ _penWidth*/, 0/*_penWidth / 2*/);
    painter->setWorldTransform(arrowTrans);
    painter->setWorldMatrixEnabled(true);

    painter->drawConvexPolygon(_arrowPoints, 3);

    painter->setWorldMatrixEnabled(false);
    painter->setWorldTransform(initTrans);
}

void qmlLine::truncateEnds() {
    if (_points.count() >= 2) {
        if (_lineStartType == LineEndArrow) {
            // truncate start
            QPointF dir = _points[0] - _points[1];
            double dirLen = len(dir);
            if (dirLen > _arrowSize) {
                dir = dir / dirLen * (dirLen - _arrowSize * 0.7); // shorten by _arrowSize + _penWidth / 2
                _points[0] = _points[1] + dir;
            }
        }

        if (_lineEndType == LineEndArrow) {
            // truncate end
            QPointF dir = _points[_points.count()-1] - _points[_points.count()-2];
            double dirLen = len(dir);
            if (dirLen > _arrowSize) {
                dir = dir / dirLen * (dirLen - _arrowSize * 0.7);
                _points[_points.count()-1] = _points[_points.count()-2] + dir;
            }
        }
    }
}

QVariant qmlLine::vertices() const { return _vertices; }


void qmlLine::setVertices(const QVariant &value) {
    _vertices = value;

    //Update the actual line points.
    _points.clear();
    QVariantList vertlist = _vertices.toList();

    for (int ci = 0; (ci + 1) < vertlist.size(); ci += 2) {
        _points.append(QPointF(vertlist.at(ci).toReal(), vertlist.at(ci+1).toReal()));
    }



    // recalculate min and max x,y
    update();

    // set relative coordinates
    for (int pi=0; pi<_points.count(); ++pi) {
        _points[pi].setX(_points[pi].x() - _minX);
        _points[pi].setY(_points[pi].y() - _minY);
    }

    Q_EMIT verticesChanged();
}
