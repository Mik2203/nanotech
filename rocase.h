#ifndef ROCASE_H
#define ROCASE_H

#include <QObject>
#include "rosystem.h"
#include "rocosts.h"
#include "rosystemcontroller.h"
#include "rosystemsolvescheduler.h"

/*
 * содержит информацию о варианте системы и включает в себя описание
 * самой системы (ROSystem), затраты на систему (ROCosts), класс анализа и
 * обработки предупреждений (ROSystemController), класс, содержащий информацию о
 * текущем ходе расчетов (ROSystemSolveScheduler)
 */

class ROCase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ROSystem* sys READ sys CONSTANT)

    Q_PROPERTY(ROCosts* costs READ costs CONSTANT)
    Q_PROPERTY(ROSystemController* sysC READ sysC CONSTANT) // по сути является сервисом, не должен быть здесь
    Q_PROPERTY(ROSystemSolveScheduler* sysSS READ sysSS CONSTANT) // по сути является сервисом, не должен быть здесь

public:
    explicit ROCase();
    ~ROCase();
    
    ROSystem* const sys() const;
    ROCosts* const costs() const;
    ROSystemController* const sysC() const;
    ROSystemSolveScheduler* const sysSS() const;
    void copyDataFrom(const ROCase* const from);


private:
    ROSystem* const _sys;
    ROCosts* _costs;
    ROSystemController* _sysC;
    ROSystemSolveScheduler* _sysSS;

signals:
    void inputChanged();

public slots:
    void reset();
    
};

#endif // ROCASE_H
