#ifndef ROSYSTEMSOLVESCHEDULER_H
#define ROSYSTEMSOLVESCHEDULER_H

#include <QObject>
#include <QFutureWatcher>

#include "rosystem.h"
#include "singleton.h"
#include "rosystemsolver.h"

/* содержит информацию о текущем состоянии расчета системы
 * (рассчитана, рассчитывается, необходим пересчет,
 * решена [рассчитана без критических ошибок]).
 * При расчете асинхронно вызывает метод солвера системы ROSystemSolver.
 */

//#define roSysSS ROSystemSolveScheduler::instance()
class ROSystemSolveScheduler : public QObject
{
    Q_OBJECT
    Q_ENUMS(SolverStatus)
    Q_PROPERTY(unsigned status READ status NOTIFY statusChanged)

    Q_PROPERTY(ROSystemSolver* solver READ solver CONSTANT)

    Q_PROPERTY(bool calculated READ calculated NOTIFY statusChanged)
    Q_PROPERTY(bool calculating READ calculating NOTIFY statusChanged)
    Q_PROPERTY(bool needUpdate READ needUpdate NOTIFY statusChanged)
    Q_PROPERTY(bool solved READ solved NOTIFY statusChanged)


public:
    enum SolverStatus {
        Calculating =   0x0001,
        Calculated =    0x0002,
        Solved =        0x0004,
        NeedUpdate =    0x0008
    };

    explicit ROSystemSolveScheduler(ROSystem* const _sys = 0, QObject *parent = 0);
    ~ROSystemSolveScheduler() { delete _solver; }

    unsigned status() const;
    ROSystemSolver* const solver() const;




    bool solved() const;
    bool calculating() const;
    bool calculated() const;
    bool needUpdate() const;

signals:
    void statusChanged();
    
public slots:
    void startSolving();
    void resetCalculated();

private slots:
    void calculationFinished();

private:
    void setStatus(unsigned status);
    ROSystemSolver* const _solver;
    QFutureWatcher<void> watcher;
    unsigned _status;
};


#endif // ROSYSTEMSOLVESCHEDULER_H
