#ifndef ROSOLUTEMODELIO_H
#define ROSOLUTEMODELIO_H

#include <QObject>
#include <QtSql/QSqlRelationalTableModel>

#include "rosolutemodel.h"
#include "rowatermodel.h"

/* класс для работы с базой данных вод.
 */

class ROSoluteModelIO : public QObject {
    Q_OBJECT

    Q_PROPERTY(ROWaterModel* waterModel READ waterModel CONSTANT)

public:
    explicit ROSoluteModelIO(QObject *parent = 0);
    

    Q_INVOKABLE bool loadFromDB(int rowIndex, ROSoluteModel* solutes);
    Q_INVOKABLE void saveToDB(const QString& name, ROSoluteModel* solutes);
    Q_INVOKABLE bool removeFromDB(int rowIndex);

    ROWaterModel* waterModel() const;

signals:
    void curSolutesRowChanged();
    
public slots:

private:
    ROWaterModel* _water;

    void asyncSaveToDB(const QString& name, ROSoluteModel* solutes);
    void asyncRemoveFromDB(int rowIndex);    
};

#endif // ROSOLUTEMODELIO_H
