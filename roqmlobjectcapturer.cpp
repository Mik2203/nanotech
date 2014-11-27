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

    if (sourceItem && _engine) {
        QImage img(sourceItem->boundingRect().toRect().size(), QImage::Format_RGB32);
        QPainter painter(&img);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);



        QDeclarativeComponent component(_engine, QUrl(itemFile));

        //        TODO
//        if (!_component->isLoading()) {
//            continueExecute();
//        } else {
//            connect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(continueExecute()));
//        }

        while(component.isLoading()) {}


        QDeclarativeItem * item = qobject_cast<QDeclarativeItem *>(component.create(QDeclarativeEngine::contextForObject(sourceItem)));

        if (!item) {
            qDebug() << "BAD ITEM:" << itemFile;
            return QImage();
        }

//        sourceItem->

        auto propIt = itemProps.constBegin();
        while (propIt != itemProps.constEnd()) {
             item->setProperty(propIt.key().toAscii(), propIt.value());
            ++propIt;
        }


        QGraphicsScene scene;
        scene.setBackgroundBrush(Qt::white);
        scene.addItem(item);
        scene.render(&painter,  sourceItem->boundingRect(),  sourceItem->boundingRect().toRect());
        scene.removeItem(item);
        delete item;
        return img;
    }
    return QImage();
}

