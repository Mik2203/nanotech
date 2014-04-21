#ifndef ROQMLOBJECTCAPTURER_H
#define ROQMLOBJECTCAPTURER_H

#include <QObject>
#include <QDeclarativeItem>
#include <QGraphicsScene>

// класс для захвата QML в картинку

class ROQMLObjectCapturer : public QObject {

    Q_OBJECT
    Q_PROPERTY(QDeclarativeItem * item READ item WRITE setItem NOTIFY itemChanged)
public:
    explicit ROQMLObjectCapturer(QDeclarativeEngine* engine, QObject *parent = 0);
    QImage getImage();
    QDeclarativeItem * item() const;
    void setItem(QDeclarativeItem *item);
    
private:
    QDeclarativeItem *_item;
    QDeclarativeEngine* _engine;

signals:
    void itemChanged();
};

#endif // ROQMLOBJECTCAPTURER_H
