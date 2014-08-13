#ifndef ROMEMBRANESMODEL_H
#define ROMEMBRANESMODEL_H

#include <QtSql/QSqlQueryModel>

#include "romembrane.h"

/* модель таблицы БД с одноименным названием.
 * Данный класс создается и инициализируется в синглтоне RODatabase.
 * Вызов populate предполагает, что база данных уже была подключена.
 */

class ROMembranesModel: public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT)

public:

    enum MembraneRoles  {
        IdRole = Qt::UserRole + 1,
        SeriesIdRole,
        SeriesRole,
        ModelRole,
        DiameterRole,
        LengthRole,
        AreaRole,
        ProductivityRole,
        RejectionRole,
        MaxFeedPressureRole,
        MaxPressureDropRole,
        MaxFeedRateRole,
        MinConcenrateToPermeateRole,
        TestSolutionNameRole,
        TestSolutionConcetrationRole,
        TestPressureRole,
        TestRecoveryRole
    };
    explicit ROMembranesModel(QObject *parent = 0);

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE QVariant get(int row, const QString & field_name) const;
    void getMembraneData(ROMembrane*& membrane, int row) const;
    Q_INVOKABLE int indexById(int id) const;
    int count() const;
    QHash<int, QByteArray> roleNames() const;

private:
    QHash<int, QByteArray> _roleNames;
};

#endif // ROMEMBRANESMODEL_H
