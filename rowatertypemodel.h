#ifndef ROWATERTYPEMODEL_H
#define ROWATERTYPEMODEL_H

#include <QObject>
#include <QtSql/QSqlRelationalTableModel>

/*
 * ����� ��� ����������� ����� ��� �� ��
 * ����� ������� �� ���� ������: ����� populate ������������, ��� ���� ������ ��� ���� ����������.
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

    void populate(); // ��������� ����������
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
