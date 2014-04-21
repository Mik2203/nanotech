#include "rosolutemodelio.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

#ifdef QT_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif


ROSoluteModelIO::ROSoluteModelIO(QObject *parent) :
    _water(new ROWaterModel(parent)), QObject(parent) {

}

bool ROSoluteModelIO::loadFromDB(int rowIndex, ROSoluteModel* solutesModel) {
#ifdef QT_DEBUG
    qDebug() << rowIndex;
#endif

    QSqlRecord rec = _water->record(rowIndex);
    ROSolutes* solutes = solutesModel->solutes();
    if (rec.count()) {
        solutes->beginChange();
        int row = 4; // ions start from 4
        for (int si = 0; si < ROSolutes::TotalIons; ++si) {
            if (si != ROSolutes::CO3 && si != ROSolutes::CO2) {
                solutes->setMeql(si, rec.value(row).toDouble());
                ++row;
            }
        }
        solutes->setPH(rec.value("pH").toDouble());
        solutes->setTemperature(rec.value("T").toDouble());
        solutes->endChange();
        return true;
    }
    return false;
}

void ROSoluteModelIO::saveToDB(const QString& name, ROSoluteModel* solutes) {
    asyncSaveToDB(name, solutes);
}

void ROSoluteModelIO::asyncSaveToDB(const QString& name, ROSoluteModel* solutesModel) {

    ROSolutes* solutes = solutesModel->solutes();

    QSqlQuery tryFind(QString("SELECT * FROM ROWater "
                            "WHERE name='%1' AND group_id='%2'").arg(name).arg(_water->USER_GROUP_ID()));
    tryFind.exec();


    QSqlQuery query;

    if (tryFind.next()) {
        // FOUND IN DB
        query.prepare(QString("UPDATE ROWater "
                      "SET NH4=?, K=?, Na=?, Mg=?, Ca=?, Fe=?, Ba=?, Sr=?, Cl=?, NO3=?, F=?, SO4=?, HCO3=?, B=?, SiO2=?, pH=?, T=?"
                      "WHERE name='%1' AND group_id='%2';").arg(name).arg(_water->USER_GROUP_ID()));

    } else {
        query.prepare("INSERT INTO ROWater (group_id, name, NH4, K, Na, Mg, Ca, Fe, Ba, Sr, Cl, NO3, F, SO4, HCO3, B, SiO2, pH, T) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        query.addBindValue(_water->USER_GROUP_ID());
        query.addBindValue(name);

    }

    for (int si = 0; si < ROSolutes::TotalIons; ++si) {
        if (si != ROSolutes::CO3 && si != ROSolutes::CO2) {
            query.addBindValue(solutes->meql(si));
        }
    }
    query.addBindValue(solutes->pH());
    query.addBindValue(solutes->temperature());
    query.exec();
    _water->update();

#ifdef QT_DEBUG
    qDebug() << "Water" << name << "was saved." << query.executedQuery() << query.lastQuery() << query.lastInsertId();
#endif

}

bool ROSoluteModelIO::removeFromDB(int rowIndex) {
    asyncRemoveFromDB(rowIndex);
    return true;
}

void ROSoluteModelIO::asyncRemoveFromDB(int rowIndex) {

    QSqlQuery query;
    query.prepare("DELETE FROM ROWater WHERE id=?");
    query.addBindValue(_water->record(rowIndex).value("id"));
    query.exec();
    _water->update();
}

ROWaterModel* ROSoluteModelIO::waterModel() const { return _water; }
