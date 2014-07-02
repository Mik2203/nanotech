#ifndef ROMEMBRANESALTERNATIVES_H
#define ROMEMBRANESALTERNATIVES_H

#include <QSqlQueryModel>

/* модель таблицы БД с одноименным названием.
 * Данный класс создается и инициализируется в синглтоне RODatabase.
 */

class ROMembranesAlternativesModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT)

public:
    explicit ROMembranesAlternativesModel(QObject *parent = 0);
    enum MembraneAlternativesRoles  {
        NanotechIdRole = Qt::UserRole + 1,
        NanotechRole,
        DowRole,
        CsmRole,
        HydranauticsRole,
        TorayRole,
        KochRole,
        DesalGERole
    };

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE QVariant get(int row, const QString & field_name) const;
    int count() const {return rowCount(); }

    QHash<int, QByteArray> roleNames() const;

private:
    QHash<int, QByteArray> _roleNames;

signals:
    
public slots:
    
};

#endif // ROMEMBRANESALTERNATIVES_H
