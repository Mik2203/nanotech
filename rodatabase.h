#ifndef RODATABASE_H
#define RODATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>

#include "singleton.h"
#include "rowatertypemodel.h"
#include "romembranesmodel.h"
#include "romembranesalternativesmodel.h"
#include "rogenericschemesmodel.h"

/*
 * класс сингтон, содержащий таблицы БД, отображенные в структуры языка С++.
 * Содержит следующие классы: ROWaterTypeModel, ROMembranesModel,
 * ROMembranesAlternativesModel, ROGenericSchemesModel отображающие
 * соответствующие таблицы БД.
 */


#define roDB RODatabase::instance()

class RODatabase : public QObject, public Singleton<RODatabase>
{
    Q_OBJECT
    Q_PROPERTY(ROWaterTypeModel* waterTypes READ waterTypes CONSTANT)
    Q_PROPERTY(ROMembranesModel* membranes READ membranes CONSTANT)
    Q_PROPERTY(ROMembranesAlternativesModel* membraneAlternatives READ membraneAlternatives CONSTANT)
    Q_PROPERTY(ROGenericSchemesModel* genericSchemes READ genericSchemes CONSTANT)

public:

    static bool setDatabase(const QString& dbname);
    ROWaterTypeModel* const waterTypes();
    ROMembranesModel* const membranes();
    ROMembranesAlternativesModel* const membraneAlternatives();
    ROGenericSchemesModel* const genericSchemes();
    const QString& errorText() const;

    
private:
    friend class Singleton<RODatabase>;
    RODatabase();
    ~RODatabase();

    bool setInstanceDatabase(const QString& dbname);

    QSqlDatabase _db;
    ROWaterTypeModel* _waterTypeModel;
    ROMembranesModel* _membranesModel;
    ROMembranesAlternativesModel* _membraneAlternativesModel;
    ROGenericSchemesModel* _genericSchemesModel;
    QString _errorText;
};


#endif // RODATABASE_H

