#include "rogenericschemesmodel.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QFile>
#include "roxmlprojectserializer.h"
#include "rounits.h"
#include "rounitstext.h"


const QVector<QStringList> ROGenericSchemesModel::_filePaths =
        QVector<QStringList>() << (QStringList() <<  // Energetics
                                   ":/generic_schemes/energo/m150-54.ntp" <<
                                   ":/generic_schemes/energo/energo-48.ntp" <<
                                   ":/generic_schemes/energo/energo-25.ntp") <<

                                  (QStringList() <<  // Medicine
                                   ":/generic_schemes/medicine/purified_water-0.5.ntp" <<
                                   ":/generic_schemes/medicine/purified_water-0.25.ntp") <<

                                  (QStringList() <<  // Electronics
                                   ":/generic_schemes/electronics/electronics-1.ntp") <<

                                  (QStringList() <<  // Beverages
                                   ":/generic_schemes/beverages/food_processing.ntp" <<
                                   ":/generic_schemes/beverages/softening.ntp") <<

                                  (QStringList() <<  // Desalination
                                   ":/generic_schemes/desalination/seawater.ntp" <<
                                   ":/generic_schemes/desalination/caspian.ntp");


ROGenericSchemesModel::ROGenericSchemesModel(QObject *parent) :
    QAbstractListModel(parent) {
    _roleNames[GroupRole] = "group";
    _roleNames[SchemesRole] = "schemes";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif
}

const char * ROGenericSchemesModel::_groupNames[] = {
    QT_TR_NOOP("Energetics"),
    QT_TR_NOOP("Medicine"),
    QT_TR_NOOP("Electronics"),
    QT_TR_NOOP("Beverages"),
    QT_TR_NOOP("Desalination")
};

QVariant ROGenericSchemesModel::data(const QModelIndex &index, int role) const {
    SchemeGroup group = static_cast<SchemeGroup>(qBound(0, index.row(), static_cast<int>(SCHEME_GROUP_COUNT)));
    switch(role) {
    case GroupRole: return tr(_groupNames[group]);
    case SchemesRole: return schemeNames(group);
    }
    return QVariant();
}

QStringList ROGenericSchemesModel::schemesByIndex(int index) const {
    SchemeGroup group = static_cast<SchemeGroup>(qBound(0, index, static_cast<int>(SCHEME_GROUP_COUNT)));
    return schemeNames(group);
}

void ROGenericSchemesModel::loadScheme(ROCase* case_, int groupIndex, int schemeIndex) {
    // TODO MANAGER!!!
    QFile schemeFile;
    // TODO CHECK!!!

    // TODO in PROJECT SERIALIZER - LOAD ONLY SYSTEM!!!
    schemeFile.setFileName(_filePaths[groupIndex][schemeIndex]);
    schemeFile.open(QFile::ReadOnly);
    ROXMLProjectSerializer ps;
    ROProject* loadProj = new ROProject();
    ps.deserialize(loadProj, QTextStream(&schemeFile));
    case_->sys()->resetSystem();
    case_->sysC()->reset();
    case_->costs()->reset();

    case_->sys()->copySystemDataFrom(loadProj->firstCase()->sys());
    case_->sysC()->copyDataFrom(loadProj->firstCase()->sysC());
    case_->costs()->copyDataFrom(loadProj->firstCase()->costs());

    delete loadProj;
    schemeFile.close();
}

int ROGenericSchemesModel::rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return SCHEME_GROUP_COUNT; }

QHash<int, QByteArray> ROGenericSchemesModel::roleNames() const { return _roleNames; }

QStringList ROGenericSchemesModel::schemeNames(ROGenericSchemesModel::SchemeGroup group) const
{
    switch (group) {
    case EnergeticsGroup:
        return QStringList() << tr("M150-54") <<
                                tr("Energo-48") <<
                                tr("Energo-25");
    case MedicineGroup:
        return QStringList() << tr("Purified water %1 %2").arg(ROUnits::convertFlowUnits(0.5)).arg(roUnitsText->flowUnitText(roUnits->flowUnits())) <<
                                tr("Purified water %1 %2").arg(ROUnits::convertFlowUnits(0.25)).arg(roUnitsText->flowUnitText(roUnits->flowUnits()));
    case ElectronicsGroup:
        return QStringList() << tr("Electronics-1");
    case BeveragesGroup:
        return QStringList() << tr("Food industry") <<
                                tr("Softening");
    case DesalinationGroup:
        return QStringList() << tr("Seawater") <<
                                tr("Caspian sea");
    }
    return QStringList();
}
