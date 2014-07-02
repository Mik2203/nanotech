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
    Q_OBJECT
    Q_PROPERTY(ROCase* case_ READ case_ WRITE setCase NOTIFY caseChanged)

public:
    explicit ROReportBuilder(ROProject* proj, QObject *parent = 0); // TODO убрать ROProject из конструктора и ROCase - сделать сервисом
    explicit ROReportBuilder() {} // FOR QML
    const QTextDocument* const build(QSizeF pageSize);
    const QTextDocument* const buildCosts(QSizeF pageSize);
    ROQMLObjectCapturer* const schemeCapturer() const;
    ~ROReportBuilder();

    void setCase(ROCase* const case_);
    ROCase* const case_() const;

signals:
    void caseChanged();
    
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


    ROProject* _proj;
    ROCase* _case;
    QSizeF _pageSize;
    QTextDocument* _doc;
    QTextCursor _cursor;

    static const int MAX_DATA_COL_NUM = 7;
    static const int V_HEADER_COUNT = 2;

    QString double2Str(double val);
    QString int2Str(double val);

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
    void insertSystemGeneralDetails();
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
