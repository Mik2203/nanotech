#ifndef ROREPORTBUILDER_H
#define ROREPORTBUILDER_H

#include <QObject>
#include <QStack>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTableCell>

#include "roproject.h"

#include "roqmlobjectcapturer.h"


/* класс для построения отчетов.
 * возвращает QTextDocument.
 */

class ROReportBuilder : public QObject
{
    Q_OBJECT
public:
    explicit ROReportBuilder();
    QTextDocument* const build(ROCase * const case_, QSizeF pageSize);
    QTextDocument* const buildCosts(ROCase * const case_, QSizeF pageSize);
    ROQMLObjectCapturer* const schemeCapturer() const;  // TODO move to other service
    ~ROReportBuilder();

public slots:

private:
    enum DataRow {
        SysHeaderRow = 0,
        SysTemperatureRow,
        SysWaterTypeRow,
        PassHeaderRow,
        PassFlowFactorRow,
        PassPowerRow,
        PassSpecificEnergyRow,
        StageHeaderRow,
        StageMembraneRow,
        ElementHeaderRow,
        ActiveAreaRow,
        AvgFluxRow,
        RecoveryRow,
        FlowFeedRow,
        FlowPermeateRow,
        FlowConcentrateRow,
        PressureFeedRow,
        PressurePermeateRow,
        PressureConcentrateRow,
        OsmPressureFeedRow,
        OsmPressurePermeateRow,
        OsmPressureConcentrateRow,
        GenericTotalRows
    };

    enum DataCol {
        ParamCol = 0,
        UnitsCol
    };

    ROCase * _case;

    QSizeF _pageSize;
    QTextDocument* _doc;
    QTextCursor _cursor;

    QString double2Str(double val);
    QString int2Str(double val);

    bool insertCapturedImage(const QString& componentPath, const QString& objectName, QHash<QString, QVariant> itemProps=QHash<QString, QVariant>());

    void insertCase();
    void insertCosts();
    void insertWarnings();
    void insertScheme();

    void moveCursorToEnd();
    void insertText(const QTextTableCell& cell, const QString& text);
    void insertText(const QTextTableCell& cell, double val);
    void insertText(const QTextTableCell& cell, int val);
    void insertText(const QString& text);
    void insertLineBreak(int n=1);

    // Costs
    void insertCostsTotalsDetails();
    void insertCostsSystemDesignDetails();
    void insertCostsChemistryDetails();
    void insertCostsWaterDetails();
    void insertCostsMaintenanceDetails();


    QTextCharFormat _tcf;
    QTextCharFormat _caseHeaderFormat;
    QTextBlockFormat _tbf;

    void storeFormat();
    void restoreFormat();
    QStack<QTextCharFormat> _tcfStack;
    QStack<QTextBlockFormat> _tbfStack;
};

#endif // ROREPORTBUILDER_H
