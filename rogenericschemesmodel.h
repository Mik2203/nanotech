#ifndef ROGENERICSCHEMESMODEL_H
#define ROGENERICSCHEMESMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "roproject.h"

/* модель для загрузки стандартных схем.
 */

class ROGenericSchemesModel : public QAbstractListModel {

    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT)
public:
    explicit ROGenericSchemesModel(QObject *parent = 0);
    enum SchemeGroup {
        EnergeticsGroup,
        MedicineGroup,
        ElectronicsGroup,
        BeveragesGroup,
        DesalinationGroup,
        SCHEME_GROUP_COUNT
    };

    enum MembraneRole  {
        GroupRole = Qt::UserRole + 1,
        SchemesRole
    };

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE QStringList schemesByIndex(int index) const;
    Q_INVOKABLE void loadScheme(ROCase* case_, int groupIndex, int schemeIndex);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int count() const { return rowCount(); }
    QHash<int, QByteArray> roleNames() const;
    
private:
    QStringList schemeNames(SchemeGroup group) const;

    QHash<int, QByteArray> _roleNames;

    static const char * _groupNames[];
    static const QVector<QStringList> _filePaths;
    
};

#endif // ROGENERICSCHEMESMODEL_H
