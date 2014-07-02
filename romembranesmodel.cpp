#include "romembranesmodel.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QStringList>

#ifdef QT_DEBUG
#include <QDebug>
#endif

ROMembranesModel::ROMembranesModel(QSqlDatabase db, QObject *parent): QSqlRelationalTableModel(parent, db) {
    _roleNames[SeriesRole] = "series";
    _roleNames[ModelRole] = "model_";
    _roleNames[DiameterRole] = "diameter";
    _roleNames[LengthRole] = "length";
    _roleNames[AreaRole] = "area";
    _roleNames[ProductivityRole] = "productivity";
    _roleNames[RejectionRole] = "rejection";
    _roleNames[PressureRole] = "pressure";
    _roleNames[TestSolutionRole] = "test_solution";
    _roleNames[MaxFeedPressureRole] = "max_feed_pressure";
    _roleNames[MaxPressureDropRole] = "max_pressure_drop";
    _roleNames[MaxFeedRateRole] = "max_feed_rate";
    _roleNames[MinConcenrateToPermeateRole] = "min_concentrate_to_permeate";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif
}

QVariant ROMembranesModel::data( const QModelIndex & index, int role) const {
    if (SeriesRole <= role && role <= MinConcenrateToPermeateRole)
        return QSqlRelationalTableModel::data(this->index(index.row(), role-SeriesRole+1));
    else
        return QSqlRelationalTableModel::data(index, role);
    //return data(index.row(), QString(roleNames()[role])); // record(index.row()).value(QString(roleNames()[role]));
}

void ROMembranesModel::populate() {
    setTable("ROMembranes");
    setRelation(1, QSqlRelation("ROMembranesSeries", "id", "series"));
    setRelation(9, QSqlRelation("ROWater", "id", "name"));
    setSort(1, Qt::AscendingOrder); // by series
    select();
}

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
        membrane = new ROMembrane(membraneRecord.value(1).toString(), // Series
                                  membraneRecord.value(2).toString(), // Model
                                  ROMembraneSize(membraneRecord.value(3).toDouble(), // Diameter
                                                 membraneRecord.value(4).toDouble()), // Length
                                  membraneRecord.value(5).toDouble(), // Area
                                  membraneRecord.value(6).toDouble(), // Productivity
                                  membraneRecord.value(7).toDouble(), // Rejection
                                  membraneRecord.value(8).toDouble()); // Pressure
    }

}

int ROMembranesModel::indexById(int id) const {
    for (int row=0; row<rowCount(); ++row) {
        if (record(row).value(0) == id)
            return row;
    }
    return -1;
}

int ROMembranesModel::count() const { return rowCount(); }

QHash<int, QByteArray> ROMembranesModel::roleNames() const { return _roleNames; }



