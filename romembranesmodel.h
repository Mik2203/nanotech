#ifndef ROMEMBRANESMODEL_H
#define ROMEMBRANESMODEL_H

#include <QtSql/QSqlRelationalTableModel>

#include "romembrane.h"

/* модель таблицы БД с одноименным названием.
 * Данный класс создается и инициализируется в синглтоне RODatabase.
 * Вызов populate предполагает, что база данных уже была подключена.
 */

class ROMembranesModel: public QSqlRelationalTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT)

public:

    enum MembraneRoles  {
        SeriesRole = Qt::UserRole + 1,
        ModelRole,
        DiameterRole,
        LengthRole,
        AreaRole,
        ProductivityRole,
        RejectionRole,
        PressureRole,
        TestSolutionRole,
        MaxFeedPressureRole,
        MaxPressureDropRole,
        MaxFeedRateRole,
        MinConcenrateToPermeateRole
    };
    explicit ROMembranesModel(QSqlDatabase db = QSqlDatabase(), QObject *parent = 0);

    void populate(); // заполнить содержимым
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
