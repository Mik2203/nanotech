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

    enum MembraneRoles  {
        GroupRole = Qt::UserRole + 1,
        SchemesRole
    };

    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE QStringList schemesByIndex(int index) const;
    Q_INVOKABLE void loadScheme(ROCase* case_, int groupIndex, int schemeIndex);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int count() const { return rowCount(); }
    QHash<int, QByteArray> roleNames() const;
    
signals:
    
public slots:

private:
    struct DataRecord {
        explicit DataRecord(int groupIndex_, const QStringList& schemes_, const QStringList& filePaths_):
            groupIndex(groupIndex_), schemes(schemes_), filePaths(filePaths_) {}
        int groupIndex;
        QStringList schemes;
        QStringList filePaths;
    };

    QList<DataRecord> _data;
    QHash<int, QByteArray> _roleNames;

    static const char * _groupNames[];
    
};

#endif // ROGENERICSCHEMESMODEL_H
