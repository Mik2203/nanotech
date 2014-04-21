#ifndef ROWATERTYPEMODEL_H
#define ROWATERTYPEMODEL_H

#include <QObject>
#include <QtSql/QSqlRelationalTableModel>

/*
 * Класс для отображения типов вод из БД
 * Класс зависит от базы данных: вызов populate предполагает, что база данных уже была подключена.
 */

class ROWaterTypeModel: public QSqlRelationalTableModel {

    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT)

public:

    enum MembraneRoles  {
        NameRole = Qt::UserRole + 1,
        MinFluxRole,
        MaxFluxRole,
        MaxRecoveryRole,
        MinConcentrate,
        FlowFactor,
        SaltPassage
    };
    explicit ROWaterTypeModel(QSqlDatabase db = QSqlDatabase(), QObject *parent = 0);

    void populate(); // заполнить содержимым
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE QVariant get(int row, const QString & field_name) const;
    int count() const;
    QHash<int, QByteArray> roleNames() const;

    static const size_t WATER_TYPE_PERMEATE = 0;

private:
    bool _populated;
    QHash<int, QByteArray> _roleNames;

    static const char * _typeNames[];
    
private slots:
    void updateModel();
};

#endif // ROWATERTYPEMODEL_H
