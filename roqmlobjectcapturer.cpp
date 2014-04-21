#include "roqmlobjectcapturer.h"
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>
#include <QDeclarativeContext>

#include "roapplication.h"

ROQMLObjectCapturer::ROQMLObjectCapturer(QDeclarativeEngine* engine, QObject *parent) :
    QObject(parent),
    _engine(engine),
    _item(nullptr)
{
}

QImage ROQMLObjectCapturer::getImage() {
    if (_item && _engine) {
        QImage img(_item->boundingRect().toRect().size(), QImage::Format_ARGB32);
        QPainter painter(&img);

#ifdef QT_DEBUG
        QDeclarativeComponent component(_engine, QUrl("qml/ROSchemeSystem.qml"));
#else
        QDeclarativeComponent component(_engine, QUrl("qrc:/qml/ROSchemeSystem.qml"));
#endif

        QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component.create(QDeclarativeEngine::contextForObject(_item))); //context
        item->setProperty("editable", false);
        item->setProperty("showWarnings", true);

        QGraphicsScene scene;
        scene.setBackgroundBrush(Qt::white);
        scene.addItem(item);
        scene.render(&painter, item->boundingRect(), item->boundingRect().toRect());
        scene.removeItem(item);
        return img;

    }
    return QImage();
}

QDeclarativeItem *ROQMLObjectCapturer::item() const { return _item; }
void ROQMLObjectCapturer::setItem(QDeclarativeItem *item) {
    _item = item;


    Q_EMIT itemChanged();
}
