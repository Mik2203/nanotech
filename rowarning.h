#ifndef ROWARNING_H
#define ROWARNING_H

#include <functional>

#include <QObject>
#include <QString>

/* содержит информацию о возникшем предупреждении и его типе
 * (критическая ошибка, предупреждение, информация),
 * а также функции выявления этих ситуаций.
 */

class ROWarning : public QObject {
    friend class ROSystemController;
    friend class ROPassController;
    friend class ROStageController;
    friend class ROFeedController;
    friend class ROElementController;

    Q_OBJECT

    Q_ENUMS(WarningType)

    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(QString what READ what NOTIFY whatChanged)
    Q_PROPERTY(WarningType type READ type CONSTANT)
public:

    enum WarningType {
        WarningCritical = 0x1,
        WarningCaution = 0x2,
        WarningInfo = 0x4,
        WarningAll = 0x7
    };


    explicit ROWarning(QObject *parent = 0); // for QML
    bool enabled() const;
    QString what() const;
    WarningType type() const;

signals:
    void enabledChanged();
    void whatChanged();
    
public slots:
    void update();

private:

    explicit ROWarning(std::function<bool()> updateFunc,
                       WarningType type,
                       std::function<QString()> updateWhatFunc,
                       QObject *parent = 0);

    bool _enabled;
    QString _what;
    const WarningType _type;
    const std::function<bool()> _updateFunc;
    const std::function<QString()> _updateWhatFunc;

};

#endif // ROWARNING_H
