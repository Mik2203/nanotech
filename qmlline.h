#ifndef QMLLINE_H
#define QMLLINE_H

#include <QDeclarativeItem>
#include <QPainter>

// класс линии для отрисовки на схеме

class qmlLine : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(LineEndType)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(LineEndType lineStartType READ lineStartType WRITE setLineStartType NOTIFY lineStartTypeChanged)
    Q_PROPERTY(LineEndType lineEndType READ lineEndType WRITE setLineEndType NOTIFY lineEndTypeChanged)
    Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth NOTIFY penWidthChanged)
    Q_PROPERTY(QVariant vertices READ vertices WRITE setVertices NOTIFY verticesChanged)

public:

    enum LineEndType { LineEndNone, LineEndArrow };

    explicit qmlLine(QDeclarativeItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    QColor color() const;
    LineEndType lineStartType() const;
    LineEndType lineEndType() const;
    QVariant vertices() const;
    int penWidth() const;


    void setColor(const QColor &color);
    void setLineStartType(LineEndType type);
    void setLineEndType(LineEndType type);
    void setVertices(const QVariant &value);
    void setPenWidth(int newWidth);

signals:
    void colorChanged();
    void lineStartTypeChanged();
    void lineEndTypeChanged();
    void verticesChanged();
    void penWidthChanged();

protected:
    void update();

protected:
    QColor _color;
    LineEndType _lineStartType;
    LineEndType _lineEndType;
    int _penWidth;

    QVariant _vertices;
    QVector<QPointF> _points;

private:
    void drawArrow(QPainter *painter, const QPointF& p0, const QPointF& p1);
    void truncateEnds();
    double _arrowSize;
    QPointF _arrowPoints[3];
    double _minX;
    double _minY;
};

QML_DECLARE_TYPE(qmlLine)

#endif // QMLLINE_H
