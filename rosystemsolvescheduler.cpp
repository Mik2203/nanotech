#include "rosystemsolvescheduler.h"

#include <QDebug>
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif
#include <QFuture>


#include "roapplication.h"





ROSystemSolveScheduler::ROSystemSolveScheduler(ROSystem* const sys, QObject *parent) :
    _status(0),
    _solver(new ROSystemSolver(sys)),
    QObject(parent)
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(calculationFinished()));
}

unsigned ROSystemSolveScheduler::status() const { return _status; }
ROSystemSolver *const ROSystemSolveScheduler::solver() const { return _solver; }

bool ROSystemSolveScheduler::solved() const { return _status & Solved; }
bool ROSystemSolveScheduler::calculating() const { return _status & Calculating; }
bool ROSystemSolveScheduler::calculated() const { return _status & Calculated; }
bool ROSystemSolveScheduler::needUpdate() const { return _status & NeedUpdate; }

void ROSystemSolveScheduler::resetCalculated() {
    if (watcher.isRunning()) {
        setStatus(NeedUpdate | status());
    } else
        setStatus(NeedUpdate);
}

void ROSystemSolveScheduler::calculationFinished() {
    if (status() & NeedUpdate) {
        setStatus(NeedUpdate | Calculated | (solver()->solved() ? Solved : 0));
    } else {
        setStatus(Calculated | (solver()->solved() ? Solved : 0));
    }
}

void ROSystemSolveScheduler::setStatus(unsigned status) {
    _status = status;
    Q_EMIT statusChanged();
}



void ROSystemSolveScheduler::startSolving()  {
    setStatus(Calculating);

    QFuture<void> future = QtConcurrent::run(solver(), &ROSystemSolver::solve);
    watcher.setFuture(future);
}
