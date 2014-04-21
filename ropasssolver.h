#ifndef ROPASSSOLVER_H
#define ROPASSSOLVER_H

#include <QObject>
#include "ropass.h"

class ROPassSolver : public QObject
{
    Q_OBJECT
public:
    explicit ROPassSolver(QObject *parent = 0);

    static void setTolerance(double value);
    static double tolerance();

signals:
    
public slots:
    static bool solve(ROPass* const pass);


private:
    static double _tolerance;
};

#endif // ROPASSSOLVER_H
