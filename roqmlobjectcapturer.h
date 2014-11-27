#ifndef ROQMLOBJECTCAPTURER_H
#define ROQMLOBJECTCAPTURER_H

#include <QObject>
#include <QDeclarativeItem>
#include <QGraphicsScene>

// класс для захвата QML в картинку

class ROQMLObjectCapturer : public QObject {

    Q_OBJECT
public:
    explicit ROQMLObjectCapturer(QDeclarativeEngine* engine, QObject *parent = 0);
    QImage getImage(const QString& itemFile, const QString& itemName, QHash<QString, QVariant> itemProps=QHash<QString, QVariant>());
    
private:
    QDeclarativeEngine* _engine;

signals:
    void itemChanged();
};

#endif // ROQMLOBJECTCAPTURER_H
