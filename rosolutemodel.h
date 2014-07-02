#ifndef QMLSOLUTESMODEL_H
#define QMLSOLUTESMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "rosolutes.h"

/* класс для работы с ROSolutes в GUI.
 */


class ROSoluteModel : public QAbstractListModel
{
    Q_OBJECT

    Q_ENUMS(RowType)

    Q_PROPERTY(ROSolutes* solutes READ solutes WRITE setSolutes NOTIFY solutesChanged)

public:

    enum SoluteRoles  {
        LabelRole = Qt::UserRole + 1,
        MglRole,
        MeqlRole,
        TypeRole
    };

    enum RowType {
        CationIonRowType = 0x1,
        OtherIonRowType = 0x2,
        AnionIonRowType = 0x4,
        TotalAnionsRowType = 0x8,
        TotalCationsRowType = 0x10,
        EntirelyTotalRowType = 0x20,
        TotalRowType = 0x38
    };

    enum TotalRows { TotalCationsRow=ROSolutes::TotalCations,
                     TotalAnionsRow=ROSolutes::TotalAnions+1,
                     TotalIonsRow=ROSolutes::TotalIons+2 };


    explicit ROSoluteModel(QObject *parent = 0);

    ROSolutes* const solutes() const;


    Q_INVOKABLE static QString shortNameByIndex(int soluteIndex);
    Q_INVOKABLE static QString fullNameByIndex(int soluteIndex);
    Q_INVOKABLE static QString complexNameByIndex(int soluteIndex);

    Q_INVOKABLE static QString shortCompoundNameByIndex(int ci);

public slots:

    double mgl(int soluteRow) const;
    double meql(int soluteRow) const;
    double value(int soluteRow, ROSolutes::Units units) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool setData(const QModelIndex & index, const QVariant & valueByIndex, int role = Qt::EditRole);
    void setMeql(int row, double value);
    void setMgl(int row, double value);
    void setValue(int row, double value, ROSolutes::Units units);

    QHash<int, QByteArray> roleNames() const;

    void setSolutes(ROSolutes* const solutes);

    void adjust(int row);
    RowType typeByRow(int row) const;
    Qt::ItemFlags flags( const QModelIndex & index ) const;

    Q_INVOKABLE static bool isEditable(int row);

    void updateTranslation();

private:
    QHash<int, QByteArray> _roleNames;
    ROSolutes* _solutes;
    static int rowToIndex(int row);

    static const char* _solutesFullNames[];
    static const char* _solutesShortNames[];
    static const char* _compoundsShortNames[];

signals:
    void solutesChanged();

private slots:
    void updateModel();
};

#endif // QMLSOLUTESMODEL_H
