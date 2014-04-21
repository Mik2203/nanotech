#include "rosolutemodel.h"
#include <math.h>
#include "romath.h"

//#include <QApplication>
#include "roapplication.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif


const char* ROSoluteModel::_solutesFullNames[] = {
    QT_TR_NOOP("Ammonium"),
    QT_TR_NOOP("Potassium"),
    QT_TR_NOOP("Sodium"),
    QT_TR_NOOP("Magnesium"),
    QT_TR_NOOP("Calcium"),
    QT_TR_NOOP("Iron"),
    QT_TR_NOOP("Barium"),
    QT_TR_NOOP("Strontium"),

    QT_TR_NOOP("Chloride"),
    QT_TR_NOOP("Nitrate"),
    QT_TR_NOOP("Fluoride"),
    QT_TR_NOOP("Sulfate"),
    QT_TR_NOOP("Carbonate"),
    QT_TR_NOOP("Bicarbonate"),

    QT_TR_NOOP("Carbon dioxide"),
    QT_TR_NOOP("Boron"),
    QT_TR_NOOP("Silica")
};


const char* ROSoluteModel::_solutesShortNames[] = {
    "NH4",
    "K",
    "Na",
    "Mg",
    "Ca",
    "Fe",
    "Ba",
    "Sr",

    "Cl",
    "NO3",
    "F",
    "SO4",
    "CO3",
    "HCO3",

    "CO2",
    "B",
    "SiO2",
};

const char* ROSoluteModel::_compoundsShortNames[] = {
    "CaSO4",
    "BaSO4",
    "SrSO4",
    "CaF2",
    "SiO2",
    "Mg(OH)2"
};

ROSoluteModel::ROSoluteModel(QObject *parent) :
    QAbstractListModel(parent), _solutes(nullptr) {

    _roleNames[LabelRole] = "label";
    _roleNames[MglRole] = "mgl";
    _roleNames[MeqlRole] = "meql";
    _roleNames[TypeRole] = "type";
#if QT_VERSION < 0x050000
    setRoleNames(_roleNames);
#endif
//

    connect(this, SIGNAL(solutesChanged()), this, SLOT(updateModel()));
    connect(roApp->translator(), SIGNAL(currentLanguageChanged()), this, SLOT(updateModel()));
}

int ROSoluteModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return TotalIonsRow+1; // 3 totals
}

bool ROSoluteModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (role == MeqlRole || role == MglRole) {
        double soluteValue = value.toDouble();
        ROSolutes::Units soluteUnits = static_cast<ROSolutes::Units>(role-LabelRole-1);
        int soluteRow = index.row();
        setValue(soluteRow, soluteValue, soluteUnits);
        return true;
    }
    return false;
}

ROSolutes* const ROSoluteModel::solutes() const { return _solutes; }

QVariant ROSoluteModel::data(const QModelIndex &index, int role) const {
    if (role == LabelRole) {
        // TODO labelByRow()!!!
        int soluteRow = index.row();
        RowType rt = typeByRow(soluteRow);
        QString label;
        switch(rt) {
        case TotalAnionsRowType: label = tr("Total anions"); break;
        case TotalCationsRowType: label = tr("Total cations"); break;
        case EntirelyTotalRowType: label = tr("TDS"); break;
        default: label = complexNameByIndex(rowToIndex(soluteRow)); break;
        }
        return label;
    } else if (role == MeqlRole || role == MglRole) {
        ROSolutes::Units soluteUnits = static_cast<ROSolutes::Units>(role-LabelRole-1);
        int soluteRow = index.row();
        double soluteValue = value(soluteRow, soluteUnits);
        return QString::number(soluteValue, 'f', 2);
    } else if (role == TypeRole) {
        return typeByRow(index.row());
    } else if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignVCenter | Qt::AlignRight);
    }
    return QVariant();
}

QString ROSoluteModel::shortNameByIndex(int soluteIndex) { return _solutesShortNames[soluteIndex]; }

QString ROSoluteModel::fullNameByIndex(int soluteIndex) {
    return tr(_solutesFullNames[soluteIndex]);
}

QString ROSoluteModel::complexNameByIndex(int soluteIndex) {
    return QString("%1 (%2)").arg(fullNameByIndex(soluteIndex)).arg(shortNameByIndex(soluteIndex));
}

QString ROSoluteModel::shortCompoundNameByIndex(int ci) { return _compoundsShortNames[ci]; }

int ROSoluteModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}

Qt::ItemFlags ROSoluteModel::flags ( const QModelIndex & index ) const {
    int sIdx = rowToIndex(index.row());
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (ROSolutes::isEditable(sIdx)) flags |= Qt::ItemIsEditable;
    return flags;
}

bool ROSoluteModel::isEditable(int row) {
    return ROSolutes::isEditable(rowToIndex(row));
}

void ROSoluteModel::updateTranslation() {
    QModelIndex top = this->index(0),
                bottom =  this->index(TotalIonsRow);
    Q_EMIT dataChanged(top, bottom);
}

int ROSoluteModel::rowToIndex(int row) {
    int index = -1;
    if (row < TotalCationsRow) {
        index = row;
    } else if (row == TotalCationsRow) {
        index = -1;
    } else if (row < TotalAnionsRow) {
        index = row - 1;
    } else if (row == TotalAnionsRow) {
        index = -1;
    } else if (row < TotalIonsRow) {
        index = row - 2;
    } else if (row == TotalIonsRow) {
        index = -1;
    }
    return index;
}

void ROSoluteModel::setMeql(int row, double value) {
    setValue(row, value, ROSolutes::Meql);
}

void ROSoluteModel::setMgl(int row, double value) {
    setValue(row, value, ROSolutes::Mgl);
}

void ROSoluteModel::setValue(int row, double value, ROSolutes::Units units) {
    if (!solutes()) return;
    RowType rt = typeByRow(row);
    switch(rt) {
    case TotalAnionsRowType: solutes()->setTotalValue(value, units, ROSolutes::Anion); break;
    case TotalCationsRowType: solutes()->setTotalValue(value, units, ROSolutes::Cation); break;
    case EntirelyTotalRowType: solutes()->setTotalValue(value, units); break;
    default: solutes()->setValue(rowToIndex(row), value, units); break;
    }
}

QHash<int, QByteArray> ROSoluteModel::roleNames() const { return _roleNames; }

void ROSoluteModel::setSolutes(ROSolutes* const solutes) {
    //if (_solutes != nullptr) disconnect(_solutes, 0, this, 0);
    _solutes = solutes; // TODO check
    connect(_solutes, SIGNAL(solutesChanged()), this, SLOT(updateModel()));
    Q_EMIT solutesChanged();
}

void ROSoluteModel::adjust(int row) {
    if (!solutes()) return;
    double totalCations = solutes()->totalValue(ROSolutes::Meql, ROSolutes::Cation);
    double totalAnions = solutes()->totalValue(ROSolutes::Meql, ROSolutes::Anion);
    if (row < TotalCationsRow) {
        if (totalCations < totalAnions)
            setMeql(row, meql(row) + (totalAnions-totalCations));
    } else if (row == TotalCationsRow) {
        if (totalCations < totalAnions)
            solutes()->setTotalValue(totalAnions, ROSolutes::Meql, ROSolutes::Cation);
    } else if (row < TotalAnionsRow) {
        if (totalAnions < totalCations)
            setMeql(row, meql(row) + (totalCations-totalAnions));
    } else if (row == TotalAnionsRow) {
        if (totalAnions < totalCations)
            solutes()->setTotalValue(totalCations, ROSolutes::Meql, ROSolutes::Anion);
    }
}

ROSoluteModel::RowType ROSoluteModel::typeByRow(int row) const {
    RowType rt = OtherIonRowType;
    if (row < TotalCationsRow) {
        rt = CationIonRowType;
    } else if (row == TotalCationsRow) {
        rt = TotalCationsRowType;
    } else if (row < TotalAnionsRow) {
        rt = AnionIonRowType;
    } else if (row == TotalAnionsRow) {
        rt = TotalAnionsRowType;
    } else if (row == TotalIonsRow) {
        rt = EntirelyTotalRowType;
    }
    return rt;
}


double ROSoluteModel::mgl(int row) const { return value(row, ROSolutes::Mgl); }
double ROSoluteModel::meql(int row) const { return value(row, ROSolutes::Meql); }
double ROSoluteModel::value(int row, ROSolutes::Units units) const {
    if (!solutes()) return 0.0;
    RowType rt = typeByRow(row);
    double value = 0.0;
    switch(rt) {
    case TotalAnionsRowType: value = solutes()->totalValue(units, ROSolutes::Anion); break;
    case TotalCationsRowType: value = solutes()->totalValue(units, ROSolutes::Cation);break;
    case EntirelyTotalRowType: value = solutes()->totalValue(units);break;
    default: value = solutes()->value(rowToIndex(row), units);break;
    }
    return value;
}



void ROSoluteModel::updateModel() {
        QModelIndex top = this->index(0);
        QModelIndex bottom = this->index(TotalIonsRow, columnCount()-1);
        Q_EMIT dataChanged(top, bottom);
}
