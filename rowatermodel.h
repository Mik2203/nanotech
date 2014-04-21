#ifndef ROWATERMODEL_H
#define ROWATERMODEL_H

#include <QSqlTableModel>


// класс, для работы с БД вод
// id групп зашиты в БД и не меняются

class ROWaterModel : public QSqlQueryModel {
    Q_OBJECT

    Q_PROPERTY(int count READ count CONSTANT)
    Q_PROPERTY(QVariant waterGroupsIds READ waterGroupsIds CONSTANT)
    Q_PROPERTY(QVariant waterGroupsNames READ waterGroupsNames NOTIFY waterGroupsNamesChanged)
    Q_PROPERTY(int USER_GROUP_ID READ USER_GROUP_ID CONSTANT)

public:

    explicit ROWaterModel(QObject *parent = 0);
    enum WaterRoles  {
        IdRole = Qt::UserRole + 1,
        GroupRole,
        NameRole,
        GroupIdRole
    };

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE QVariant get(int row, const QString & field_name) const;
    QVariant waterGroupsIds() const;
    QVariant waterGroupsNames() const;
    int count() const;
    void update();

    QHash<int, QByteArray> roleNames() const;
    int USER_GROUP_ID() const { return _USER_GROUP_ID; }

signals:
    void waterGroupsNamesChanged();

private:
    QHash<int, QByteArray> _roleNames;

    static const char * _waterNames[];
    static const int _USER_GROUP_ID = 0;
};

#endif // ROWATERMODEL_H
