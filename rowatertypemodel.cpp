#include "rowatertypemodel.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QStringList>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "roapplication.h"

const char * ROWaterTypeModel::_typeNames[] = {
    QT_TR_NOOP("RO Permeate [SDI < 1]"),
    QT_TR_NOOP("UF Permeate [SDI < 2]"),
    QT_TR_NOOP("Well water [SDI < 3]"),
    QT_TR_NOOP("Surface supply [SDI < 3]"),
    QT_TR_NOOP("Surface supply [SDI < 5]"),
    QT_TR_NOOP("Tertiary effluent (Microfiltration) [SDI < 3]"),
    QT_TR_NOOP("Tertiary effluent (Conventional) [SDI < 5]"),
    QT_TR_NOOP("Seawater (Well/MF) [SDI < 3]"),
    QT_TR_NOOP("Seawater (Open intake) [SDI < 5]")
};

ROWaterTypeModel::ROWaterTypeModel(QObject *parent): QSqlRelationalTableModel(parent) {
    _roleNames[NameRole] = "name";
    _roleNames[MinFluxRole] = "min_flux";
    _roleNames[MaxFluxRole] = "max_flux";
    _roleNames[MaxRecoveryRole] = "max_recovery";
    _roleNames[MinConcentrate] = "min_concentrate";
    _roleNames[FlowFactor] = "flow_factor";
    _roleNames[SaltPassage] = "salt_passage";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif

    setTable("ROWaterType");
    select();

    connect(roApp->translator(), SIGNAL(currentLanguageChanged()), this, SLOT(updateModel()));
}

QVariant ROWaterTypeModel::data( const QModelIndex & index, int role) const {
    if (role == NameRole) {
        return tr(_typeNames[index.row()]);
    } else if (NameRole < role && role <= SaltPassage)
        return QSqlRelationalTableModel::data(this->index(index.row(), role-NameRole+1));
    else
        return QSqlRelationalTableModel::data(index);
}

QVariant ROWaterTypeModel::get(int row, const QString & field_name) const {
    if (field_name == "name") {
        return tr(_typeNames[row]);
    }
    return record(row).value(field_name);
}

int ROWaterTypeModel::count() const { return rowCount(); }

QHash<int, QByteArray> ROWaterTypeModel::roleNames() const { return _roleNames; }

void ROWaterTypeModel::updateModel() {
    QModelIndex top = this->index(0, 0);
    QModelIndex bottom = this->index(rowCount()-1, columnCount()-1);
    Q_EMIT dataChanged(top, bottom);
}
