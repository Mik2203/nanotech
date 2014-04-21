#include "romembranesalternativesmodel.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

ROMembranesAlternativesModel::ROMembranesAlternativesModel(QObject *parent) :
    QSqlQueryModel(parent) {
//    QHash<int, QByteArray> roles;
    _roleNames[NanotechIdRole] = "id";
    _roleNames[NanotechRole] = "nanotech";
    _roleNames[DowRole] = "dow";
    _roleNames[CsmRole] = "csm";
    _roleNames[HydranauticsRole] = "hydranautics";
    _roleNames[TorayRole] = "toray";
    _roleNames[KochRole] = "koch";
    _roleNames[DesalGERole] = "desalGE";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif
//    setRoleNames(roles);


    setQuery("SELECT ROMembranesAlternatives.nanotech_id as id, series || ' ' || model as nanotech, dow, csm, hydranautics, toray, koch, desal_ge as desalGE FROM ROMembranesAlternatives "
             "LEFT JOIN ROMembranes ON ROMembranesAlternatives.nanotech_id = ROMembranes.id "
             "LEFT JOIN ROMembranesSeries ON ROMembranes.series_id = ROMembranesSeries.id");
#ifdef QT_DEBUG
    qDebug() << "ERROR!" << this->lastError().text();
    qDebug() << this->rowCount();
    qDebug() << this->data(this->index(2,2), DowRole);
#endif
}

QVariant ROMembranesAlternativesModel::data( const QModelIndex & index, int role) const {
    if (NanotechIdRole <= role && role <= DesalGERole) {
        return QSqlQueryModel::data(this->index(index.row(), role-NanotechIdRole));
    }
    else
        return QSqlQueryModel::data(index, role);
}

QVariant ROMembranesAlternativesModel::get(int row, const QString &field_name) const {
    return record(row).value(field_name);
}

QHash<int, QByteArray> ROMembranesAlternativesModel::roleNames() const { return _roleNames; }
