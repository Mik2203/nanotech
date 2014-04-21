#ifndef ROSYSTEMCONTROLLER_H
#define ROSYSTEMCONTROLLER_H

#include <QObject>

#include "rowarning.h"
#include "ropasscontroller.h"
#include "rofeedcontroller.h"
#include "rosystem.h"
#include "rosystemsolvescheduler.h"


/* содержит логику выявления и сигналирования об ошибках и предупреждениях,
 * возникающих на системе. Предупреждения представлены классом ROWarning.
 */

class ROSystemController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ROWarning* waterTypeChosen READ waterTypeChosen CONSTANT)
    Q_PROPERTY(ROWarning* badInputData READ badInputData CONSTANT)
    Q_PROPERTY(bool hasAnyCriticalWarnings READ hasAnyCriticalWarnings NOTIFY hasAnyCriticalWarningsChanged)
    Q_PROPERTY(bool hasAnyCautionWarnings READ hasAnyCautionWarnings NOTIFY hasAnyCautionWarningsChanged)

public:
    explicit ROSystemController(ROSystem* sys, ROSystemSolveScheduler* sysSS, QObject *parent = 0);
    explicit ROSystemController(); // FOR QML
    ~ROSystemController();
    void copyDataFrom(const ROSystemController* const from);

    ROSystemSolveScheduler* const sysSS() const;

    int passCCount() const;
    Q_INVOKABLE ROPassController* passC(int passCIndex) const;
    Q_INVOKABLE ROPassController* passC(ROPass* pass) const;

    int feedCCount() const;
    Q_INVOKABLE ROFeedController* feedC(int feedCIndex) const;
    Q_INVOKABLE ROFeedController* feedC(ROFeed* feed) const;

    // WARNINGS
    ROWarning* const waterTypeChosen() const;  // CRITICAL
    ROWarning* const badInputData() const; // CRITICAL

    Q_INVOKABLE QStringList allWarningsMessages(unsigned type/* = ROWarning::WarningAll*/) const;

    bool hasAnyCriticalWarnings() const;
    bool hasAnyCautionWarnings() const;
    void setSys(ROSystem* const);


    ROSystem* const sys() const;
signals:
    void hasAnyCriticalWarningsChanged();
    void hasAnyCautionWarningsChanged();
    void inputChanged();
    
public slots:
    //bool calc();
    void updateWarnings();
    void reset();


private slots:
    void updatePasses();
    void updateFeeds();
    
private:
    ROSystem* _sys;
    ROSystemSolveScheduler* _sysSS;
    QList<ROPassController*> _passControllers;
    QList<ROFeedController*> _feedControllers;

    // WARNINGS
    ROWarning* _waterTypeChosen;
    ROWarning* _badInputData;
};

#endif // ROSYSTEMCONTROLLER_H

