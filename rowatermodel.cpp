#include "rowatermodel.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStringList>

#include "roapplication.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

ROWaterModel::ROWaterModel(QObject *parent) : QSqlQueryModel(parent) {
    _roleNames[IdRole] = "id";
    _roleNames[GroupRole] = "group";
    _roleNames[NameRole] = "name";
    _roleNames[GroupIdRole] = "group_id";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif
    update();

    connect(roApp->translator(), SIGNAL(currentLanguageChanged()), this, SIGNAL(waterGroupsNamesChanged()));
}

QVariant ROWaterModel::data( const QModelIndex & index, int role) const {
    if (role == GroupRole) {
        int groupId = this->record(index.row()).value("group_id").toInt();
        return tr(_waterNames[groupId]);
    } else if (role == IdRole || role == NameRole || role == GroupIdRole)
        return QSqlQueryModel::data(this->index(index.row(), role-IdRole));
    else
        return QSqlQueryModel::data(index);
}

QVariant ROWaterModel::get(int row, const QString& field_name) const {
    if (field_name == "group") {
        int groupId = this->record(row).value("group_id").toInt();
        return tr(_waterNames[groupId]);
    }
    return record(row).value(field_name);
}


// TODO WATER GROUP MODEL
QVariant ROWaterModel::waterGroupsIds() const {
    QList<QVariant> ids;
    QSqlQuery selectGroups("SELECT id FROM ROWaterGroups ORDER BY id");
    while (selectGroups.next()) {
        ids << selectGroups.value(0);
    }
    return ids;
}


QVariant ROWaterModel::waterGroupsNames() const {
    QList<QVariant> names;
    QList<QVariant> ids = waterGroupsIds().toList();
    Q_FOREACH(QVariant id, ids) {
        // Проверка на id
        names << tr(_waterNames[id.toInt()]);
    }

    return names;
}


int ROWaterModel::count() const { return rowCount(); }

void ROWaterModel::update() {
    setQuery(QSqlQuery("SELECT ROWater.id, ROWaterGroups.name AS 'group', ROWater.name, ROWaterGroups.id AS 'group_id', "
                 "ROWater.NH4, ROWater.K, ROWater.Na, ROWater.Mg, ROWater.Ca, "
                 "ROWater.Fe, ROWater.Ba, ROWater.Sr, ROWater.Cl, ROWater.NO3, "
                 "ROWater.F, ROWater.SO4, ROWater.HCO3, ROWater.B, ROWater.SiO2, "
                 "ROWater.T, ROWater.pH "
             "FROM ROWater, ROWaterGroups "
             "WHERE ROWater.group_id = ROWaterGroups.id"));
}

QHash<int, QByteArray> ROWaterModel::roleNames() const { return _roleNames; }





const char * ROWaterModel::_waterNames[] = {
    QT_TR_NOOP("User"),
    QT_TR_NOOP("Seawater"),
    QT_TR_NOOP("Well Water"),
    QT_TR_NOOP("Surface Supply"),
    QT_TR_NOOP("Test")
};
