#include "roreportbuilder.h"

#include <QTextCursor>
#include <QTextBlock>
#include <QTextList>
#include <QTextTableCell>

#include "rodatabase.h"
#include "rounits.h"
#include "rounitstext.h"
#include "roapplication.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

ROReportBuilder::ROReportBuilder()
{
    _tcf.setFontPointSize(8.25);
}

// TODO рефактор: удалить дублирование кода
QTextDocument* const ROReportBuilder::build(ROCase * const case_, QSizeF pageSize) {
    _doc = new QTextDocument();
    _cursor = QTextCursor(_doc);
    _case = case_;
    _pageSize = pageSize;
    QTextCharFormat caseHeaderFormat;
    caseHeaderFormat.setFontUnderline(true);
    caseHeaderFormat.setFontWeight(QFont::Bold);
    caseHeaderFormat.setFontPointSize(16);
    _cursor.insertText(tr("Case %1 details").arg(roApp->projectManager()->proj()->caseIndex(case_)+1), caseHeaderFormat);
    insertLineBreak();
//    moveCursorToEnd();
//    _cursor.insertBlock();
//    moveCursorToEnd();
    insertCase(case_);
//    qDebug() << _doc->toHtml(QByteArray());
    return _doc;
}

QTextDocument *const ROReportBuilder::buildCosts(ROCase * const case_, QSizeF pageSize) {
    _case = case_;
    _doc = new QTextDocument();
    _cursor = QTextCursor(_doc);
    _pageSize = pageSize;
    _doc->setPageSize(pageSize);
    QTextCharFormat caseHeaderFormat;
    caseHeaderFormat.setFontUnderline(true);
    caseHeaderFormat.setFontWeight(QFont::Bold);
    caseHeaderFormat.setFontPointSize(16);
    _cursor.insertText(tr("Case %1 Costs details").arg(roApp->projectManager()->proj()->caseIndex(case_)+1), caseHeaderFormat);
    insertLineBreak(2);
    moveCursorToEnd();
//    _cursor.insertBlock();
//    moveCursorToEnd();
//    _cursor.insertBlock();

//    moveCursorToEnd();

    insertCosts(case_);
    return _doc;
}



void ROReportBuilder::insertWarnings(ROCase *case_) {
    bool hasCriticalWarnings = case_->sysC()->hasAnyCriticalWarnings();
    bool hasCautionWarnings = case_->sysC()->hasAnyCautionWarnings();

    if (hasCriticalWarnings || hasCautionWarnings) {
        // init table
        QTextTableFormat tableFormat;
        tableFormat.setBorder(0.0);
        QVector<QTextLength> constraints;
        constraints << QTextLength(QTextLength::FixedLength, 50); // first column with image
        tableFormat.setColumnWidthConstraints(constraints);
        int rows = 1, columns = 2;
        QTextTable *table = _cursor.insertTable(rows, columns, tableFormat);

        QString warningImagePath;
        QStringList warningsMessages;
        if (hasCriticalWarnings) {
            // critical warnings
            warningImagePath = ":/images/warning_critical.png";
            warningsMessages = case_->sysC()->allWarningsMessages(ROWarning::WarningCritical);
        } else if (hasCautionWarnings) {
            // caution warnings
            warningImagePath = ":/images/warning_caution.png";
            warningsMessages = case_->sysC()->allWarningsMessages(ROWarning::WarningCaution);
        }

        QImage warningImage = QImage(warningImagePath).scaledToWidth(40);
        table->cellAt(0, 0).firstCursorPosition().insertImage(warningImage);
        insertText(table->cellAt(0,1), warningsMessages.join("\n"));
    } else {
        insertText(tr("No warnings."));
    }
//    moveCursorToEnd();
}

void ROReportBuilder::insertScheme() {
    QHash<QString, QVariant> itemProps;
    itemProps["editable"] = false;
    itemProps["showWarnings"] = true;

    insertCapturedImage("qrc:/qml/ROSchemeSystem.qml", "scheme", itemProps);
}

void ROReportBuilder::insertCase(ROCase *case_) {
    if (case_->sysC()->sysSS()->calculated()) {
        insertScheme();
        insertLineBreak();
//        insertDataTableNew();

        QTextCharFormat headerFormat;
        headerFormat.setFontPointSize(14);
        headerFormat.setFontWeight(QFont::Bold);

        QHash<QString, QVariant> itemProps;
        itemProps["element"] = qVariantFromValue((QObject *) case_->sys());

//        insertImage("qrc:/qml/results-page/util/ElementTitle.qml", "systemDetailsHeader");
        _cursor.insertText(tr("System details"), headerFormat);
        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/system/Common.qml", "systemDetailsCommon");
        insertLineBreak();
//        insertCapturedImage("qrc:/qml/results-page/system/Hydrodynamics.qml", "systemDetailsHydrodynamics");
//        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/common/Solubility.qml", "systemDetailsSolubility", itemProps);
        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/common/Scaling.qml", "systemDetailsScaling", itemProps);
//        insertLineBreak();

//        insertImage("qrc:/qml/results-page/pass/Data.qml", "passDetails");
        insertLineBreak();
//        insertImage("qrc:/qml/results-page/util/ElementTitle.qml", "passDetailsHeader");
        _cursor.insertText(tr("Passes details"), headerFormat);
        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/pass/Common.qml", "passDetailsCommon");
        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/pass/Flows.qml", "passDetailsStreams");
//        insertLineBreak();

//        insertImage("qrc:/qml/results-page/stage/Data.qml", "stageDetails");
        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/util/ElementTitle.qml", "stageDetailsHeader");
        insertLineBreak();
        _cursor.insertText(tr("Stages details"), headerFormat);
        insertLineBreak();
        insertCapturedImage("qrc:/qml/results-page/stage/Common.qml", "stageDetailsCommon");
        insertLineBreak(2);
        insertCapturedImage("qrc:/qml/results-page/stage/Flows.qml", "stageDetailsStreams");
        insertLineBreak();

        insertLineBreak();
        _cursor.insertText(tr("Elements details"), headerFormat);
        insertLineBreak();
        int row = 0;
        while (insertCapturedImage("qrc:/qml/results-page/element/Common.qml", QString("elementDetailsCommonRow%1").arg(row))) {
            insertLineBreak(2);
            ++row;
        }
    } else {
        insertText("Calculate first.");
    }
    insertLineBreak(2);
    insertWarnings(case_);
}

void ROReportBuilder::insertCosts(ROCase *case_) {
    Q_UNUSED(case_);
    insertCostsTotalsDetails();
    insertLineBreak(3);
    insertCostsSystemDesignDetails();
    insertLineBreak(3);
    insertCostsChemistryDetails();
    insertLineBreak(3);
    insertCostsWaterDetails();
    insertLineBreak(3);
    insertCostsMaintenanceDetails();
}

void ROReportBuilder::moveCursorToEnd() {
    _cursor.setPosition(_doc->lastBlock().position());
}

void ROReportBuilder::storeFormat() {
    _tcfStack.push(_tcf);
    _tbfStack.push(_tbf);
}

void ROReportBuilder::restoreFormat() {
    _tcf = _tcfStack.pop();
    _tbf = _tbfStack.pop();
}

QString ROReportBuilder::double2Str(double val) {
    return QString::number(val, 'f', 2);
}

QString ROReportBuilder::int2Str(double val) {
    return QString::number(val);
}

bool ROReportBuilder::insertCapturedImage(const QString &componentPath, const QString &objectName, QHash<QString, QVariant> itemProps)
{
    QImage image = roApp->schemeCapturer()->getImage(componentPath, objectName, itemProps);
//    if (image.width() > _pageSize.width())
//        image = image.scaledToWidth(_pageSize.width(), Qt::SmoothTransformation);
    if (!image.isNull())
        _cursor.insertImage(image);
    return !image.isNull();
}

void ROReportBuilder::insertText(const QTextTableCell &cell, const QString &text) {
    _cursor = cell.firstCursorPosition();
    insertText(text);
}

void ROReportBuilder::insertText(const QTextTableCell &cell, double val) {
    insertText(cell, double2Str(val));
}

void ROReportBuilder::insertText(const QTextTableCell &cell, int val) {
    insertText(cell, int2Str(val));
}

void ROReportBuilder::insertText(const QString &text) {
    _cursor.setBlockCharFormat(_tcf);
    _cursor.setBlockFormat(_tbf);
    _cursor.insertText(text);
}


void ROReportBuilder::insertLineBreak(int n) {
    for (int i=0; i<n; ++i)
        _cursor.insertBlock();
}


ROReportBuilder::~ROReportBuilder() {
}


void ROReportBuilder::insertCostsTotalsDetails() {
    ROCosts* costs = _case->costs();

    storeFormat();
    _tcf.setFontWeight(QFont::Bold);
    //_tcf.setUnderlineStyle(QTextCharFormat::DashUnderline);
    insertText(tr("Total Costs (%1 rub/year, %2 rub/m³)")
               .arg(double2Str(costs->totalCostsYear()))
               .arg(double2Str(costs->totalWaterCostsYear())));
    restoreFormat();
    insertLineBreak(2);

    // init table
    storeFormat();
    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_None);

    QTextTable *table = _cursor.insertTable(4, 2, tableFormat);

    // vertical headers
    insertText(table->cellAt(0, 0), tr("Year permeate (m³)"));
    insertText(table->cellAt(1, 0), tr("System cost (rub)"));
    insertText(table->cellAt(2, 0), tr("System set cost (rub)"));
    insertText(table->cellAt(3, 0), tr("System with set cost (rub)"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(0, 1), costs->permeateYearAmount());
    insertText(table->cellAt(1, 1), costs->systemCost());
    insertText(table->cellAt(2, 1), costs->systemSetCostYear());
    insertText(table->cellAt(3, 1), costs->systemWithYearSetCost());

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertCostsSystemDesignDetails() {
    ROCosts* costs = _case->costs();

    storeFormat();
    _tcf.setFontWeight(QFont::Bold);
    //_tcf.setUnderlineStyle(QTextCharFormat::DashUnderline);
    insertText(tr("System Design Costs (%1 rub/year, %2 rub/m³)")
               .arg(double2Str(costs->totalCostsYearSystem()))
               .arg(double2Str(costs->totalWaterCostsYearSystem())));
    restoreFormat();
    insertLineBreak(2);

    // init table
    storeFormat();
    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_None);

    QTextTable *table = _cursor.insertTable(5, 7, tableFormat);

    // vertical headers
    insertText(table->cellAt(1, 0), tr("Microfiltes requirement"));
    insertText(table->cellAt(2, 0), tr("Membrane elements requirement"));
    insertText(table->cellAt(4, 0), tr("Energy requirement"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(0, 1), tr("Items by element"));
    insertText(table->cellAt(0, 2), tr("Items by system"));
    insertText(table->cellAt(0, 3), tr("Change times a year"));
    insertText(table->cellAt(0, 4), tr("Items of a year"));
    insertText(table->cellAt(0, 5), tr("Item cost (rub)"));
    insertText(table->cellAt(0, 6), tr("Year costs (rub)"));

    insertText(table->cellAt(1, 1), costs->microfiltersMembraneCount());
    insertText(table->cellAt(1, 2), costs->microfiltersSystemCount());
    insertText(table->cellAt(1, 3), costs->microfiltersYearChangeFrequency());
    insertText(table->cellAt(1, 4), costs->microfiltersYearCount());
    insertText(table->cellAt(1, 5), costs->microfilterCost());
    insertText(table->cellAt(1, 6), costs->microfiltersYearCost());

    insertText(table->cellAt(2, 2), _case->sys()->elementsCount());
    insertText(table->cellAt(2, 3), costs->membranesYearChangeFrequency());
    insertText(table->cellAt(2, 4), costs->membranesYearCount());
    insertText(table->cellAt(2, 5), costs->membraneCost());
    insertText(table->cellAt(2, 6), costs->membranesYearCost());

    insertText(table->cellAt(3, 1), tr("Energy by 1 m³ (kW/h)"));
    insertText(table->cellAt(3, 2), tr("Energy by system (kW/h)"));
    insertText(table->cellAt(3, 4), tr("Energy of a year"));
    insertText(table->cellAt(3, 5), tr("Energy cost (rub/(kW/h))"));

    insertText(table->cellAt(4, 1), _case->sys()->specificEnergy());
    insertText(table->cellAt(4, 2), _case->sys()->power());
    insertText(table->cellAt(4, 4), costs->electricEnergyYearAmount());
    insertText(table->cellAt(4, 5), costs->electricEnergyCost());
    insertText(table->cellAt(4, 6), costs->electricEnergyYearCost());

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertCostsChemistryDetails() {
    ROCosts* costs = _case->costs();

    storeFormat();
    _tcf.setFontWeight(QFont::Bold);
    //_tcf.setUnderlineStyle(QTextCharFormat::DashUnderline);
    insertText(tr("Chemistry Costs (%1 rub/year, %2 rub/m³)")
               .arg(double2Str(costs->totalCostsYearScaling()))
               .arg(double2Str(costs->totalWaterCostsYearScaling())));
    restoreFormat();
    insertLineBreak(2);

    // init table
    storeFormat();
    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_None);

    QTextTable *table = _cursor.insertTable(8, 7, tableFormat);

    // vertical headers
    insertText(table->cellAt(1, 0), tr("Citric acid requirement"));
    insertText(table->cellAt(2, 0), tr("H2SO4 requirement"));
    insertText(table->cellAt(3, 0), tr("Trilon B requirement"));
    insertText(table->cellAt(4, 0), tr("Alkali requirement"));
    insertText(table->cellAt(6, 0), tr("Antiscalant requirement"));
    insertText(table->cellAt(7, 0), tr("Acid-antiscalant requirement"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(0, 1), tr("Sink (m³)"));
    insertText(table->cellAt(0, 2), tr("Sink (kg)"));
    insertText(table->cellAt(0, 3), tr("Sink times a year"));
    insertText(table->cellAt(0, 4), tr("Mass of a year (kg)"));
    insertText(table->cellAt(0, 5), tr("Item cost (rub/kg)"));
    insertText(table->cellAt(0, 6), tr("Year costs (rub)"));

    insertText(table->cellAt(1, 1), costs->washingSolutionVolume());
    insertText(table->cellAt(1, 2), costs->citricAcidAmount());
    insertText(table->cellAt(1, 3), costs->washingFrequency());
    insertText(table->cellAt(1, 4), costs->citricAcidYearAmount());
    insertText(table->cellAt(1, 5), costs->citricAcidCost());
    insertText(table->cellAt(1, 6), costs->citricAcidYearCost());

    insertText(table->cellAt(2, 1), costs->washingSolutionVolume());
    insertText(table->cellAt(2, 2), costs->H2SO4Amount());
    insertText(table->cellAt(2, 3), costs->washingFrequency());
    insertText(table->cellAt(2, 4), costs->H2SO4YearAmount());
    insertText(table->cellAt(2, 5), costs->H2SO4Cost());
    insertText(table->cellAt(2, 6), costs->H2SO4YearCost());

    insertText(table->cellAt(3, 1), costs->washingSolutionVolume());
    insertText(table->cellAt(3, 2), costs->trilonBAmount());
    insertText(table->cellAt(3, 3), costs->washingFrequency());
    insertText(table->cellAt(3, 4), costs->trilonBYearAmount());
    insertText(table->cellAt(3, 5), costs->trilonBCost());
    insertText(table->cellAt(3, 6), costs->trilonBYearCost());

    insertText(table->cellAt(4, 1), costs->washingSolutionVolume());
    insertText(table->cellAt(4, 2), costs->alkaliAmount());
    insertText(table->cellAt(4, 3), costs->washingFrequency());
    insertText(table->cellAt(4, 4), costs->alkaliYearAmount());
    insertText(table->cellAt(4, 5), costs->alkaliCost());
    insertText(table->cellAt(4, 6), costs->alkaliYearCost());

    insertText(table->cellAt(5, 1), tr("Rate (mg/l)"));
    insertText(table->cellAt(5, 2), tr("Rate by sys (kg/h)"));

    insertText(table->cellAt(6, 1), costs->antiscalantAmount());
    insertText(table->cellAt(6, 2), costs->antiscalantSystemAmount());
    insertText(table->cellAt(6, 4), costs->antiscalantYearAmount());
    insertText(table->cellAt(6, 5), costs->antiscalantCost());
    insertText(table->cellAt(6, 6), costs->antiscalantYearCost());

    insertText(table->cellAt(7, 1), costs->acidAntiscalantAmount());
    insertText(table->cellAt(7, 2), costs->acidAntiscalantSystemAmount());
    insertText(table->cellAt(7, 4), costs->acidAntiscalantYearAmount());
    insertText(table->cellAt(7, 5), costs->acidAntiscalantCost());
    insertText(table->cellAt(7, 6), costs->acidAntiscalantYearCost());

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertCostsWaterDetails() {
    ROCosts* costs = _case->costs();

    storeFormat();
    _tcf.setFontWeight(QFont::Bold);
    //_tcf.setUnderlineStyle(QTextCharFormat::DashUnderline);
    insertText(tr("Water Costs (%1 rub/year, %2 rub/m³)")
               .arg(double2Str(costs->totalCostsYearWater()))
               .arg(double2Str(costs->totalWaterCostsYearWater())));
    restoreFormat();
    insertLineBreak(2);

    // init table
    storeFormat();
    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_None);

    QTextTable *table = _cursor.insertTable(3, 5, tableFormat);

    // vertical headers
    insertText(table->cellAt(1, 0), tr("Raw water"));
    insertText(table->cellAt(2, 0), tr("Concentrate drop"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(0, 1), tr("Rate (m³/h)"));
    insertText(table->cellAt(0, 2), tr("Volume by year (m³)"));
    insertText(table->cellAt(0, 3), tr("Cost (rub/m³)"));
    insertText(table->cellAt(0, 4), tr("Year costs (rub)"));

    insertText(table->cellAt(1, 1), _case->sys()->feed()->rate());
    insertText(table->cellAt(1, 2), costs->rawWaterYearAmount());
    insertText(table->cellAt(1, 3), costs->rawWaterCost());
    insertText(table->cellAt(1, 4), costs->rawWaterYearCost());

    insertText(table->cellAt(2, 1), _case->sys()->concentrate()->rate());
    insertText(table->cellAt(2, 2), costs->concentrateDropYearAmount());
    insertText(table->cellAt(2, 3), costs->concentrateDropCost());
    insertText(table->cellAt(2, 4), costs->concentrateDropYearCost());

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertCostsMaintenanceDetails() {
    ROCosts* costs = _case->costs();

    storeFormat();
    _tcf.setFontWeight(QFont::Bold);
    //_tcf.setUnderlineStyle(QTextCharFormat::DashUnderline);
    insertText(tr("Maintenance Costs (%1 rub/year, %2 rub/m³)")
               .arg(double2Str(costs->totalCostsYearMaintenance()))
               .arg(double2Str(costs->totalWaterCostsYearMaintenance())));
    restoreFormat();
    insertLineBreak(2);

    // init table
    storeFormat();
    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_None);

    QTextTable *table = _cursor.insertTable(5, 6, tableFormat);

    // vertical headers
    insertText(table->cellAt(1, 0), tr("Staff salary"));
    insertText(table->cellAt(2, 0), tr("Accruals"));
    insertText(table->cellAt(4, 0), tr("Deductions"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(0, 1), tr("Salary (rub/month)"));
    insertText(table->cellAt(0, 2), tr("Staff count"));
    insertText(table->cellAt(0, 3), tr("Year salary (rub/month)"));
    insertText(table->cellAt(0, 4), tr("Overhead rate (%)"));
    insertText(table->cellAt(0, 5), tr("Year costs (rub)"));

    insertText(table->cellAt(1, 1), costs->staffSalary());
    insertText(table->cellAt(1, 2), costs->staffCount());
    insertText(table->cellAt(1, 3), costs->staffYearSalary());
    insertText(table->cellAt(1, 4), costs->overheadRate() * 100.0);
    insertText(table->cellAt(1, 5), costs->staffYearCost());

    insertText(table->cellAt(2, 1), costs->accruals());
    insertText(table->cellAt(2, 2), costs->staffCount());
    insertText(table->cellAt(2, 3), costs->accrualsYear());
    insertText(table->cellAt(2, 4), costs->overheadRate() * 100.0);
    insertText(table->cellAt(2, 5), costs->accrualsYearCost());

    insertText(table->cellAt(3, 4), tr("System lifetime (years)"));

    insertText(table->cellAt(4, 4), costs->systemLifetime());
    insertText(table->cellAt(4, 5), costs->deductionsYearCost());

    moveCursorToEnd();
    restoreFormat();
}







\
