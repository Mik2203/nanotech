#ifndef ROFEEDCONTROLLER_H
#define ROFEEDCONTROLLER_H

#include <QObject>

#include "roflow.h"
#include "rowarning.h"
#include "rosystemcontroller.h"

/* содержит логику выявления и сигналирования об ошибках и предупреждениях,
 * возникающих при анализе входной воды. Предупреждения представлены классом ROWarning.
 */

class ROFeedController : public QObject {

    Q_OBJECT
    Q_PROPERTY(ROFlow* feed READ feed CONSTANT)
    Q_PROPERTY(ROSystemController* sysC READ sysC CONSTANT)

    // WARNINGS
    Q_PROPERTY(ROWarning* ionsBalanced READ ionsBalanced CONSTANT)
    Q_PROPERTY(ROWarning* ionsFilled READ ionsFilled CONSTANT)

    Q_PROPERTY(bool hasAnyCriticalWarnings READ hasAnyCriticalWarnings NOTIFY hasAnyCriticalWarningsChanged)
    Q_PROPERTY(bool hasAnyCautionWarnings READ hasAnyCautionWarnings NOTIFY hasAnyCautionWarningsChanged)

public:
    explicit ROFeedController(ROFlow* feed, ROSystemController* sysC);
    explicit ROFeedController(); // for QML
    void copyDataFrom(const ROFeedController* const from);

    ROFlow* const feed() const;

    // WARNINGS
    ROWarning* const ionsBalanced() const; // CAUTION
    ROWarning* const ionsFilled() const; // CRITICAL

    bool hasAnyCriticalWarnings() const;
    bool hasAnyCautionWarnings() const;

    QStringList allWarningsMessages(ROWarning::WarningType type/* = ROWarning::WarningAll*/) const;

    ROSystemController* const sysC() const;

signals:
    void hasAnyCriticalWarningsChanged();
    void hasAnyCautionWarningsChanged();
    void inputChanged();
    
public slots:
    void updateWarnings();

private:
    ROFlow* _feed;

    ROSystemController* const _sysC;

    // WARNINGS
    ROWarning* _ionsFilled; // CRITICAL
    ROWarning* _ionsBalanced; // CAUTION

    
};

#endif // ROFEEDCONTROLLER_H
