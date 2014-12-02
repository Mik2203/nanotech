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
    _engine(engine) {
}

QImage ROQMLObjectCapturer::getImage(const QString& itemFile, const QString& itemName, QHash<QString, QVariant> itemProps) {
    QDeclarativeItem * sourceItem = _engine->findChild<QDeclarativeItem*>(itemName);

    if (!sourceItem || !_engine)
        return QImage();

    QDeclarativeComponent component(_engine, QUrl(itemFile));
    QDeclarativeItem * item = qobject_cast<QDeclarativeItem *>(component.create(QDeclarativeEngine::contextForObject(sourceItem)));

    if (!item)
        return QImage();

    // set item properties
    auto propIt = itemProps.constBegin();
    while (propIt != itemProps.constEnd()) {
        item->setProperty(propIt.key().toAscii(), propIt.value());
        ++propIt;
    }


    QRectF targetRect = sourceItem->boundingRect();

    // Инициализация региона захватываемого элемента из виртуальной сцены.
    QRectF sourceRect;
    QDeclarativeItem* targetItem = item->findChild<QDeclarativeItem*>(itemName);
    if (targetItem) {
        // Если в виртуальной сцене присутствует захватываемый элемент,
        // то берется его относительный регион внутри сцены.
        sourceRect = targetItem->sceneBoundingRect();
    } else {
        // Иначе берется регион из реальной сцены.
        sourceRect = targetRect;
    }


    QImage img(targetRect.toRect().size(), QImage::Format_RGB32);
    QPainter painter(&img);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);



    QGraphicsScene scene;
    scene.setBackgroundBrush(Qt::white);
    scene.addItem(item);
    scene.render(&painter, targetRect,  sourceRect);
    scene.removeItem(item);
    delete item;
    return img;
}

