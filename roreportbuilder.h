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
 * использует QTextDocument и QPrinter для печати на принтер и в pdf.
 */

class ROReportBuilder : public QObject
{

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
//    QTextFrame *_rootFrame;
    QTextCursor _cursor;

    static const int MAX_DATA_COL_NUM = 7;
    static const int V_HEADER_COUNT = 2;

    QString double2Str(double val);
    QString int2Str(double val);

    void insertCapturedImage(const QString& componentPath, const QString& objectName, QHash<QString, QVariant> itemProps=QHash<QString, QVariant>());

    void insertCase(ROCase* case_);
    void insertCosts(ROCase* case_);
    void insertWarnings(ROCase *case_);
    void insertScheme();

    void insertPageBreak();
    void moveCursorToEnd();
    void insertText(const QTextTableCell& cell, const QString& text);
    void insertText(const QTextTableCell& cell, double val);
    void insertText(const QTextTableCell& cell, int val);
    void insertText(const QString& text);
    void insertFlowData(QTextTable * table, const QTextTableCell& startCell, ROFlow* flow, bool ions=true);
    QTextTable * insertDataTable(int rows, int datacols, qreal datacolLength=55.0);
    void insertLineBreak(int n=1);
    void insertDataTableNew();

    // System
    void insertSystemCommon();
    void insertSystemDesignDetails();
    void insertSystemFlowDetails();
    void insertSystemScalingDetails();

    // Stages
    void insertStagesDesignDetails();
    void insertStagesFlowDetails();

    // Elements
    void insertElementsDesignDetails();
    void insertElementsFlowDetails();

    // Costs
    void insertCostsTotalsDetails();
    void insertCostsSystemDesignDetails();
    void insertCostsChemistryDetails();
    void insertCostsWaterDetails();
    void insertCostsMaintenanceDetails();


    QTextCharFormat _tcf;
    QTextBlockFormat _tbf;

    void storeFormat();
    void restoreFormat();
    QStack<QTextCharFormat> _tcfStack;
    QStack<QTextBlockFormat> _tbfStack;
};

#endif // ROREPORTBUILDER_H
