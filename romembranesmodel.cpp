#include "romembranesmodel.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QStringList>

#ifdef QT_DEBUG
#include <QDebug>
#endif

ROMembranesModel::ROMembranesModel(QObject *parent):
    QSqlQueryModel(parent) {

    _roleNames[IdRole] = "id";
    _roleNames[SeriesRole] = "series";
    _roleNames[ModelRole] = "model_";
    _roleNames[DiameterRole] = "diameter";
    _roleNames[LengthRole] = "length";
    _roleNames[AreaRole] = "area";
    _roleNames[ProductivityRole] = "productivity";
    _roleNames[RejectionRole] = "rejection";
    _roleNames[MaxFeedPressureRole] = "max_feed_pressure";
    _roleNames[MaxPressureDropRole] = "max_pressure_drop";
    _roleNames[MaxFeedRateRole] = "max_feed_rate";
    _roleNames[MinConcenrateToPermeateRole] = "min_concentrate_to_permeate";
    _roleNames[TestSolutionNameRole] = "test_solution_name";
    _roleNames[TestSolutionConcetrationRole] = "test_solution_concentration";
    _roleNames[TestPressureRole] = "test_pressure";
    _roleNames[TestRecoveryRole] = "test_recovery";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif
    setQuery("SELECT ROMembranes.id as id, ROMembranesSeries.series as series, model, diameter, length, area, productivity, rejection, "
             "max_feed_pressure, max_pressure_drop, max_feed_rate, min_concentrate, "
             "ROMembranesTests.solution_short_name as test_solution_name, "
             "ROMembranesTests.solution_concentration as test_solution_concentration, "
             "ROMembranesTests.pressure as test_pressure, "
             "ROMembranesTests.recovery as test_recovery "
             "FROM ROMembranes "
             "LEFT JOIN ROMembranesTests ON ROMembranes.test_id = ROMembranesTests.id "
             "LEFT JOIN ROMembranesSeries ON ROMembranes.series_id = ROMembranesSeries.id");
#ifdef QT_DEBUG
    if (this->lastError().isValid()) {
        qDebug() << "ERROR!" << this->lastError().text();
        qDebug() << this->rowCount();
    }
#endif


}

QVariant ROMembranesModel::data( const QModelIndex & index, int role) const {
    if (SeriesRole <= role && role <= TestRecoveryRole)
        return QSqlQueryModel::data(this->index(index.row(), role-IdRole));

    return QSqlQueryModel::data(index, role);
}

//void ROMembranesModel::populate() {
//    setTable("ROMembranes");
//    setRelation(1, QSqlRelation("ROMembranesSeries", "id", "series"));
//    setRelation(9, QSqlRelation("ROMembranesTests", "id", "summary"));
//    setSort(1, Qt::AscendingOrder); // by series
//    select();
//    qDebug() << record(0);
//}

QVariant ROMembranesModel::get(int row, const QString & field_name) const {
    /*
      К именам, которые не могут быть представлены именами, совпадающими с именами в БД,
      в конце подставляется символ '_'. Например "model" в QML - зарезервированое слово, поэтому
      используется "model_", но доступ к БД должен по прежнему осуществляться через "model".
      Поэтому символ '_' должен быть удален.
      */
    if (field_name.endsWith('_'))
        return record(row).value(QString(field_name).remove('_'));
    else
        return record(row).value(field_name);
}

void ROMembranesModel::getMembraneData(ROMembrane*& membrane, int row) const {
    if (membrane) {
        delete membrane;
        membrane = nullptr;
    }

    QSqlRecord membraneRecord = record(row);
    if (!membraneRecord.isEmpty()) {
        membrane = new ROMembrane(membraneRecord.value("series").toString(), // Series
                                  membraneRecord.value("model").toString(), // Model
                                  ROMembraneSize(membraneRecord.value("diameter").toDouble(), // Diameter
                                                 membraneRecord.value("length").toDouble()), // Length
                                  membraneRecord.value("area").toDouble(), // Area
                                  membraneRecord.value("productivity").toDouble(), // Productivity
                                  membraneRecord.value("rejection").toDouble(), // Rejection
                                  membraneRecord.value("test_pressure").toDouble()); // Pressure
    }

}

int ROMembranesModel::indexById(int id) const {
    for (int row=0; row<rowCount(); ++row) {
        if (record(row).value("id") == id)
            return row;
    }
    return -1;
}

int ROMembranesModel::count() const { return rowCount(); }

QHash<int, QByteArray> ROMembranesModel::roleNames() const { return _roleNames; }



