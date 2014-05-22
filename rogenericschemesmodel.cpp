#include "rogenericschemesmodel.h"

#include <QDebug>
#include <QFile>
#include "roxmlprojectserializer.h"


ROGenericSchemesModel::ROGenericSchemesModel(QObject *parent) :
    QAbstractListModel(parent) {
    _roleNames[GroupRole] = "group";
    _roleNames[SchemesRole] = "schemes";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif

    if (_data.count() == 0) { // STATIC INITIALIZER

        QStringList schemes;
        QStringList filePaths;

        // ENERGO
        schemes << QString::fromLocal8Bit("M150-54")
                << QString::fromLocal8Bit("Энерго-48")
                << QString::fromLocal8Bit("Энерго-25");

        filePaths << ":/generic_schemes/energo/m150-54.ntp"
                  << ":/generic_schemes/energo/energo-48.ntp"
                  << ":/generic_schemes/energo/energo-25.ntp";
        _data << DataRecord(0, schemes, filePaths);

        schemes.clear();
        filePaths.clear();

        // Medicine
        schemes << QString::fromLocal8Bit("Вода очищенная 0.5 м3/ч")
                << QString::fromLocal8Bit("Вода очищенная.0.25 м3/ч");
        filePaths << ":/generic_schemes/medicine/purified_water-0.5.ntp"
                  << ":/generic_schemes/medicine/purified_water-0.25.ntp";
        _data << DataRecord(1, schemes, filePaths);

        schemes.clear();
        filePaths.clear();

        // Electronics
        schemes << QString::fromLocal8Bit("Электроника-1");
        filePaths << ":/generic_schemes/electronics/electronics-1.ntp";
        _data << DataRecord(2, schemes, filePaths);

        schemes.clear();
        filePaths.clear();

        // Beverages
        schemes << QString::fromLocal8Bit("Пищевая промышленность")
                << QString::fromLocal8Bit("Умягчение");
        filePaths << ":/generic_schemes/beverages/food_processing.ntp"
                  << ":/generic_schemes/beverages/softening.ntp";
        _data << DataRecord(3, schemes, filePaths);

        schemes.clear();
        filePaths.clear();

        // Desalination
        schemes << QString::fromLocal8Bit("Морская вода")
                << QString::fromLocal8Bit("Каспийское море");
        filePaths << ":/generic_schemes/desalination/seawater.ntp"
                  << ":/generic_schemes/desalination/caspian.ntp";
        _data << DataRecord(4, schemes, filePaths);
    }
}

const char * ROGenericSchemesModel::_groupNames[] = {
    QT_TR_NOOP("Energetics"),
    QT_TR_NOOP("Medicine"),
    QT_TR_NOOP("Electronics"),
    QT_TR_NOOP("Beverages"),
    QT_TR_NOOP("Desalination")
};

QVariant ROGenericSchemesModel::data(const QModelIndex &index, int role) const {
    int row = qBound(0, index.row(), _data.count()-1);
    switch(role) {
    case GroupRole: return tr(_groupNames[_data[row].groupIndex]);
    case SchemesRole: return _data[row].schemes;
    }
    return QVariant();
}

QStringList ROGenericSchemesModel::schemesByIndex(int index) const {
    int row = qBound(0, index, _data.count()-1);
    return _data[row].schemes;
}

void ROGenericSchemesModel::loadScheme(ROCase* case_, int groupIndex, int schemeIndex) {
    // TODO MANAGER!!!
    QFile schemeFile;
    // TODO CHECK!!!

    // TODO in PROJECT SERIALIZER - LOAD ONLY SYSTEM!!!
    schemeFile.setFileName(_data[groupIndex].filePaths[schemeIndex]);
    schemeFile.open(QFile::ReadOnly);
    ROXMLProjectSerializer ps;
    ROProject* loadProj = new ROProject();
    ps.deserialize(loadProj, QTextStream(&schemeFile));
    case_->sys()->resetSystem();
    case_->sys()->copySystemDataFrom(loadProj->firstCase()->sys());
    case_->sysC()->copyDataFrom(loadProj->firstCase()->sysC());

    delete loadProj;
    schemeFile.close();
}

int ROGenericSchemesModel::rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return _data.count(); }

QHash<int, QByteArray> ROGenericSchemesModel::roleNames() const { return _roleNames; }
