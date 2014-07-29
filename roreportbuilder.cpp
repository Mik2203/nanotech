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

ROReportBuilder::ROReportBuilder(ROProject* proj, QObject *parent) :
    QObject(parent), _proj(proj), _doc(new QTextDocument()){
    _cursor = QTextCursor(_doc);
    _case = proj->firstCase();
    _tcf.setFontPointSize(7.0);
}

// TODO рефактор: удалить дублирование кода
const QTextDocument* const ROReportBuilder::build(QSizeF pageSize) {
    _doc->clear();
    _cursor.setPosition(0);
    _pageSize = pageSize;
    _doc->setPageSize(pageSize);
    QTextCharFormat caseHeaderFormat;
    caseHeaderFormat.setFontUnderline(true);
    caseHeaderFormat.setFontWeight(QFont::Bold);
    _cursor.insertText(tr("Case %1 data").arg(_proj->caseIndex(_case)+1), caseHeaderFormat);
    _cursor.insertBlock();
    moveCursorToEnd();
    _cursor.insertBlock();
    moveCursorToEnd();
    insertCase(_case);
    //qDebug() << _doc->toHtml();
    return _doc;
}

const QTextDocument *const ROReportBuilder::buildCosts(QSizeF pageSize) {
    _doc->clear();
    _cursor.setPosition(0);
    _pageSize = pageSize;
    _doc->setPageSize(pageSize);
    QTextCharFormat caseHeaderFormat;
    caseHeaderFormat.setFontUnderline(true);
    caseHeaderFormat.setFontWeight(QFont::Bold);
    _cursor.insertText(tr("Case %1 Costs data").arg(_proj->caseIndex(_case)+1), caseHeaderFormat);
    _cursor.insertBlock();
    moveCursorToEnd();
    _cursor.insertBlock();
    moveCursorToEnd();
    insertCosts(_case);
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
    moveCursorToEnd();
}

void ROReportBuilder::insertScheme() {
    QImage capturedScheme = roApp->schemeCapturer()->getImage();
    if (capturedScheme.width() > _pageSize.width())
        capturedScheme = capturedScheme.scaledToWidth(_pageSize.width(), Qt::SmoothTransformation);
    _cursor.insertImage(capturedScheme);
}

void ROReportBuilder::insertCase(ROCase *case_) {
    insertWarnings(case_);

    insertLineBreak(2);
    if (case_->sysC()->sysSS()->calculated()) {
        insertScheme();
        insertLineBreak(2);
        insertDataTableNew();
    } else {
        insertText("Calculate first.");
    }
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

void ROReportBuilder::insertDataTableNew() {
    insertText(tr("System Details"));
    insertLineBreak();
    insertSystemGeneralDetails();
    insertLineBreak(2);
    insertSystemDesignDetails();
    insertLineBreak(2);
    insertSystemFlowDetails();
    insertLineBreak(2);
    insertSystemScalingDetails();
    insertLineBreak(2);

    insertText(tr("Stages Details"));
    insertLineBreak(2);
    insertStagesDesignDetails();
    insertLineBreak(2);
    insertStagesFlowDetails();
    insertLineBreak(2);

    insertText(tr("Elements Details"));
    insertLineBreak(2);
    insertElementsDesignDetails();
    insertLineBreak(2);
    insertElementsFlowDetails();

    _cursor.movePosition(QTextCursor::NextBlock);
}

void ROReportBuilder::insertSystemGeneralDetails() {
    // init table
    storeFormat();

    QTextTable *table = insertDataTable(3, 1, 150.0);

    // vertical headers
    insertText(table->cellAt(1, 0), tr("Temperature"));
    insertText(table->cellAt(1, 1), roUnitsText->temperatureUnitText(roUnits->temperatureUnits()));

    insertText(table->cellAt(2, 0), tr("Water type"));

    _tbf.setAlignment(Qt::AlignRight);

    ROSystem* sys = case_()->sys();
    insertText(table->cellAt(1, 2), ROUnits::convertTemperatureUnits(sys->temperature()));
    insertText(table->cellAt(2, 2), roDB->waterTypes()->get(sys->waterTypeIndex(), "name").toString());

    restoreFormat();
    moveCursorToEnd();
}

void ROReportBuilder::moveCursorToEnd() {
    _cursor.movePosition(QTextCursor::End);
}


void ROReportBuilder::insertSystemDesignDetails() {
    insertText(tr("Design Details"));
    insertLineBreak();

    // init table
    storeFormat();
    QTextTable *table = insertDataTable(7, 1 + case_()->sys()->passCount());

    // vertical headers
    insertText(table->cellAt(1, 0), tr("Power"));
    insertText(table->cellAt(1, 1), tr("kW"));

    insertText(table->cellAt(2, 0), tr("Specific Energy"));
    insertText(table->cellAt(2, 1), tr("kWh/m³"));

    insertText(table->cellAt(3, 0), tr("Flow Factor"));

    insertText(table->cellAt(4, 0), tr("Active Area"));
    insertText(table->cellAt(4, 1), tr("m²"));

    insertText(table->cellAt(5, 0), tr("Average Flux"));
    insertText(table->cellAt(5, 1), roUnitsText->fluxUnitText(roUnits->fluxUnits()));

    insertText(table->cellAt(6, 0), tr("Recovery"));
    insertText(table->cellAt(6, 1), tr("%"));



    _tbf.setAlignment(Qt::AlignRight);
    // system data
    int col = 2;
    ROSystem* sys = case_()->sys();
    insertText(table->cellAt(0, col), tr("System"));
    insertText(table->cellAt(1, col), sys->power());
    insertText(table->cellAt(2, col), sys->specificEnergy());
    insertText(table->cellAt(4, col), sys->activeArea());
    insertText(table->cellAt(5, col), ROUnits::convertFluxUnits(sys->averageFlux()));
    insertText(table->cellAt(6, col), sys->recovery() * 100.0);

    ++col;
    for (int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass = sys->pass(pi);
        insertText(table->cellAt(0, col), tr("Pass %1").arg(pi+1));
        insertText(table->cellAt(1, col), pass->power());
        insertText(table->cellAt(2, col), pass->specificEnergy());
        insertText(table->cellAt(3, col), pass->flowFactor());
        insertText(table->cellAt(4, col), pass->activeArea());
        insertText(table->cellAt(5, col), ROUnits::convertFluxUnits(pass->averageFlux()));
        insertText(table->cellAt(6, col), pass->recovery() * 100.0);
        ++col;
    }

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertSystemFlowDetails() {
    insertText(tr("Flow Details"));
    insertLineBreak();
    // init table
    storeFormat();

    ROSystem* sys = case_()->sys();
    QVector<int> filledIons = sys->filledIons();
    QTextTable *table = insertDataTable(6 + filledIons.count(), 1 + sys->passCount() * 3);

    // vertical headers
    insertText(table->cellAt(2, 0), tr("Flow Rate"));
    insertText(table->cellAt(2, 1), roUnitsText->flowUnitText(roUnits->flowUnits()));

    insertText(table->cellAt(3, 0), tr("Osm. Pressure"));
    insertText(table->cellAt(3, 1), roUnitsText->pressureUnitText(roUnits->pressureUnits()));

    insertText(table->cellAt(4, 0), tr("Pressure"));
    insertText(table->cellAt(4, 1), roUnitsText->pressureUnitText(roUnits->pressureUnits()));

    insertText(table->cellAt(5, 0), tr("TDS"));
    insertText(table->cellAt(5, 1), tr("mg/l"));

    int row=6;
    Q_FOREACH(const int& si, filledIons) {
        insertText(table->cellAt(row, 0), ROSoluteModel::complexNameByIndex(si));
        insertText(table->cellAt(row, 1), tr("mg/l"));
        ++row;
    }


    auto insertFlowData = [this, &table, &filledIons](int col, ROFlow* flow) {
        int row=2;
        insertText(table->cellAt(row++, col), flow->rate());
        insertText(table->cellAt(row++, col), flow->osmoticPressure());
        insertText(table->cellAt(row++, col), flow->pressure());
        insertText(table->cellAt(row++, col), flow->solutes()->totalValueMgl());

        Q_FOREACH(const int& si, filledIons)
            insertText(table->cellAt(row++, col), flow->solutes()->mgl(si));
    };

    _tbf.setAlignment(Qt::AlignCenter);
    insertText(table->cellAt(0, 2), tr("System"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(1, 2), tr("Raw Feed"));
    insertFlowData(2, sys->feed());

    for(int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass= sys->pass(pi);
        int startCol = 3 + 3 * pi;

        _tbf.setAlignment(Qt::AlignHCenter);
        insertText(table->cellAt(0, startCol+1), tr("Pass %1").arg(pi+1));

        _tbf.setAlignment(Qt::AlignRight);
        insertText(table->cellAt(1, startCol), tr("Feed"));
        insertFlowData(startCol, pass->feed());

        insertText(table->cellAt(1, startCol+1), tr("Permeate"));
        insertFlowData(startCol+1, pass->permeate());

        insertText(table->cellAt(1, startCol+2), tr("Concentrate"));
        insertFlowData(startCol+2, pass->concentrate());
    }


    restoreFormat();
    moveCursorToEnd();
}

void ROReportBuilder::insertSystemScalingDetails() {
    insertText(tr("Scaling Details"));
    insertLineBreak();
    // init table
    storeFormat();

    ROSystem* sys = case_()->sys();
    QVector<int> saturatedCompounds = sys->saturatedCompounds();
    QTextTable *table = insertDataTable(6 + saturatedCompounds.count(), 7);

    // vertical headers
    insertText(table->cellAt(2, 0), tr("pH"));
    insertText(table->cellAt(3, 0), tr("Ionic Strenght"));
    insertText(table->cellAt(4, 0), tr("LSI"));
    insertText(table->cellAt(5, 0), tr("S&DSI"));

    int row=6;
    Q_FOREACH(const int& si, saturatedCompounds) {
        insertText(table->cellAt(row, 0), ROSoluteModel::shortCompoundNameByIndex(si));
        insertText(table->cellAt(row, 1), "%");
        ++row;
    }


    auto insertScalingData = [this, &table, &saturatedCompounds](int col, ROFlow* flow) {
        int row=2;
        insertText(table->cellAt(row++, col), flow->solutes()->pH());
        insertText(table->cellAt(row++, col), flow->solutes()->ionicStrength());
        insertText(table->cellAt(row++, col), flow->solutes()->lsi());
        insertText(table->cellAt(row++, col), flow->solutes()->sdsi());

        Q_FOREACH(const int& si, saturatedCompounds)
            insertText(table->cellAt(row++, col), flow->solutes()->saturation(static_cast<ROSolutes::ScalingCompound>(si)) * 100.0);
    };

    _tbf.setAlignment(Qt::AlignCenter);
    insertText(table->cellAt(0, 2), tr("System"));

    _tbf.setAlignment(Qt::AlignRight);
    insertText(table->cellAt(1, 2), tr("Raw Feed"));
    insertScalingData(2, sys->feed());

    for(int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass= sys->pass(pi);
        int startCol = 3 + 3 * pi;

        _tbf.setAlignment(Qt::AlignHCenter);
        insertText(table->cellAt(0, startCol+1), tr("Pass %1").arg(pi+1));

        _tbf.setAlignment(Qt::AlignRight);
        insertText(table->cellAt(1, startCol), tr("Feed"));
        insertScalingData(startCol, pass->feed());

        insertText(table->cellAt(1, startCol+1), tr("Permeate"));
        insertScalingData(startCol+1, pass->permeate());

        insertText(table->cellAt(1, startCol+2), tr("Concentrate"));
        insertScalingData(startCol+2, pass->concentrate());
    }


    restoreFormat();
    moveCursorToEnd();
}

void ROReportBuilder::insertStagesDesignDetails() {
    insertText(tr("Design Details"));
    insertLineBreak();

    ROSystem* sys = case_()->sys();
    // init table
    storeFormat();
    QTextTable *table = insertDataTable(8, sys->stagesCount()); // не делим, т.к. не более 8 стадий по условию

    // vertical headers
    insertText(table->cellAt(2, 0), tr("Element type"));

    insertText(table->cellAt(3, 0), tr("Vessels count"));

    insertText(table->cellAt(4, 0), tr("Elements count"));

    insertText(table->cellAt(5, 0), tr("Active Area"));
    insertText(table->cellAt(5, 1), tr("m²"));

    insertText(table->cellAt(6, 0), tr("Average Flux"));
    insertText(table->cellAt(6, 1), roUnitsText->fluxUnitText(roUnits->fluxUnits()));

    insertText(table->cellAt(7, 0), tr("Recovery"));
    insertText(table->cellAt(7, 1), tr("%"));




    // system data
    int col = 2;
    for (int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass = sys->pass(pi);
        //table->mergeCells(0, col, 1, pass->stageCount()); - не работает почему то - берем среднее просто
        _tbf.setAlignment(Qt::AlignHCenter);
        insertText(table->cellAt(0, col + pass->stageCount() / 2), tr("Pass %1").arg(pi+1));
        _tbf.setAlignment(Qt::AlignRight);
        for (int si=0; si<pass->stageCount(); ++si) {
            ROStage* stage = pass->stage(si);
            insertText(table->cellAt(1, col), tr("Stage %1").arg(si+1));
            insertText(table->cellAt(2, col), stage->membrane()->seriesAndModel());
            insertText(table->cellAt(3, col), stage->vesselCount());
            insertText(table->cellAt(4, col), stage->elementsPerVesselCount());
            insertText(table->cellAt(5, col), stage->activeArea());
            insertText(table->cellAt(6, col), ROUnits::convertFluxUnits(stage->averageFlux()));
            insertText(table->cellAt(7, col), stage->recovery() * 100.0);
            ++col;
        }
    }

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertStagesFlowDetails() {
    insertText(tr("Flow Details"));
    // init table
    storeFormat();

    ROSystem* sys = case_()->sys();
    QVector<int> filledIons = sys->filledIons();

    auto insertNewTable = [&](int stagesCount) -> QTextTable * {
        moveCursorToEnd();
        insertLineBreak(2);

        storeFormat();
        _tbf.setAlignment(Qt::AlignLeft);

        QTextTable *table = insertDataTable(6 + filledIons.count(), stagesCount * 3);

        // vertical headers
        insertText(table->cellAt(2, 0), tr("Flow Rate"));
        insertText(table->cellAt(2, 1), roUnitsText->flowUnitText(roUnits->flowUnits()));

        insertText(table->cellAt(3, 0), tr("Osm. Pressure"));
        insertText(table->cellAt(3, 1), roUnitsText->pressureUnitText(roUnits->pressureUnits()));

        insertText(table->cellAt(4, 0), tr("Pressure"));
        insertText(table->cellAt(4, 1), roUnitsText->pressureUnitText(roUnits->pressureUnits()));

        insertText(table->cellAt(5, 0), tr("TDS"));
        insertText(table->cellAt(5, 1), tr("mg/l"));

        int row=6;
        Q_FOREACH(const int& si, filledIons) {
            insertText(table->cellAt(row, 0), ROSoluteModel::complexNameByIndex(si));
            insertText(table->cellAt(row, 1), tr("mg/l"));
            ++row;
        }

        restoreFormat();

        return table;
    };

    int stageRestNum = sys->stagesCount();
    QTextTable *table = insertNewTable(qMin(2, stageRestNum));

    int stageTableNum = 0;
    for(int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass= sys->pass(pi);

        for(int si=0; si<pass->stageCount(); ++si) {
            ROStage* stage = pass->stage(si);

            if (stageTableNum == 2) {
                table = insertNewTable(qMin(2, stageRestNum));
                stageTableNum = 0;
            }

            int startCol = 2 + 3 * stageTableNum;

            _tbf.setAlignment(Qt::AlignRight);
            insertText(table->cellAt(0, startCol), tr("Pass %1:").arg(pi+1));
            _tbf.setAlignment(Qt::AlignLeft);
            insertText(table->cellAt(0, startCol+1), tr("Stage %1").arg(si+1));

            _tbf.setAlignment(Qt::AlignRight);
            insertText(table->cellAt(1, startCol), tr("Feed"));
            insertFlowData(table, table->cellAt(2, startCol), stage->feed());

            insertText(table->cellAt(1, startCol+1), tr("Permeate"));
            insertFlowData(table, table->cellAt(2, startCol+1), stage->permeate());

            insertText(table->cellAt(1, startCol+2), tr("Concentrate"));
            insertFlowData(table, table->cellAt(2, startCol+2), stage->concentrate());

            ++stageTableNum;
            --stageRestNum;
        }

    }


    restoreFormat();
    moveCursorToEnd();
}

void ROReportBuilder::insertElementsDesignDetails() {
    insertText(tr("Design Details"));

    ROSystem* sys = case_()->sys();
    // init table
    storeFormat();


    auto insertNewTable = [&](int elementsCount) -> QTextTable * {
        moveCursorToEnd();
        insertLineBreak(2);

        storeFormat();
        _tbf.setAlignment(Qt::AlignLeft);

        QTextTable *table = insertDataTable(5, elementsCount); // не делим, т.к. не более 8 стадий по условию

        insertText(table->cellAt(2, 0), tr("Active Area"));
        insertText(table->cellAt(2, 1), tr("m²"));

        insertText(table->cellAt(3, 0), tr("Average Flux"));
        insertText(table->cellAt(3, 1), roUnitsText->fluxUnitText(roUnits->fluxUnits()));

        insertText(table->cellAt(4, 0), tr("Recovery"));
        insertText(table->cellAt(4, 1), tr("%"));

        restoreFormat();

        return table;
    };


    int elementsRestNum = sys->elementsCount();
    QTextTable *table = insertNewTable(qMin(7, elementsRestNum));

    int elementsTableNum = 0;
    for(int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass= sys->pass(pi);

        for(int si=0; si<pass->stageCount(); ++si) {
            ROStage* stage = pass->stage(si);

            for(int ei=0; ei<stage->elementsPerVesselCount(); ++ei) {
                ROElement* element = stage->element(ei);

                if (elementsTableNum == 7) {
                    table = insertNewTable(qMin(7, elementsRestNum));
                    elementsTableNum = 0;
                }

                int col = 2 + elementsTableNum;

                _tbf.setAlignment(Qt::AlignLeft);
                insertText(table->cellAt(0, col), tr("Pass %1: Stage %2:").arg(pi+1).arg(si+1));

                insertText(table->cellAt(1, col), tr("Element %1").arg(ei+1));
                _tbf.setAlignment(Qt::AlignRight);
                insertText(table->cellAt(2, col), element->activeArea());
                insertText(table->cellAt(3, col), ROUnits::convertFluxUnits(element->averageFlux()));
                insertText(table->cellAt(4, col), element->recovery() * 100.0);

                ++elementsTableNum;
                --elementsRestNum;
            }
        }

    }

    moveCursorToEnd();
    restoreFormat();

}

void ROReportBuilder::insertElementsFlowDetails() {
    insertText(tr("Flow Details"));
//    insertLineBreak();
    // init table
    storeFormat();

    ROSystem* sys = case_()->sys();

    auto insertNewTable = [&](int elementsCount) -> QTextTable * {
        moveCursorToEnd();
        insertLineBreak(2);

        storeFormat();
        _tbf.setAlignment(Qt::AlignLeft);

        QTextTable *table = insertDataTable(6, elementsCount * 3);

        // vertical headers
        insertText(table->cellAt(2, 0), tr("Flow Rate"));
        insertText(table->cellAt(2, 1), roUnitsText->flowUnitText(roUnits->flowUnits()));

        insertText(table->cellAt(3, 0), tr("Osm. Pressure"));
        insertText(table->cellAt(3, 1), roUnitsText->pressureUnitText(roUnits->pressureUnits()));

        insertText(table->cellAt(4, 0), tr("Pressure"));
        insertText(table->cellAt(4, 1), roUnitsText->pressureUnitText(roUnits->pressureUnits()));

        insertText(table->cellAt(5, 0), tr("TDS"));
        insertText(table->cellAt(5, 1), tr("mg/l"));

        restoreFormat();

        return table;
    };

    int elementsRestNum = sys->elementsCount();
    QTextTable *table = insertNewTable(qMin(2, elementsRestNum));

    int elementsTableNum = 0;
    for(int pi=0; pi<sys->passCount(); ++pi) {
        ROPass* pass= sys->pass(pi);

        for(int si=0; si<pass->stageCount(); ++si) {
            ROStage* stage = pass->stage(si);

            for(int ei=0; ei<stage->elementsPerVesselCount(); ++ei) {
                ROElement* element = stage->element(ei);

                if (elementsTableNum == 2) {
                    table = insertNewTable(qMin(2, elementsRestNum));
                    elementsTableNum = 0;
                }

                int startCol = 2 + 3 * elementsTableNum;

                _tbf.setAlignment(Qt::AlignRight);
                insertText(table->cellAt(0, startCol), tr("Pass %1:").arg(pi+1));
                _tbf.setAlignment(Qt::AlignHCenter);
                insertText(table->cellAt(0, startCol+1), tr("Stage %1:").arg(si+1));
                _tbf.setAlignment(Qt::AlignLeft);
                insertText(table->cellAt(0, startCol+2), tr("Element %1").arg(ei+1));

                _tbf.setAlignment(Qt::AlignRight);
                insertText(table->cellAt(1, startCol), tr("Feed"));
                insertFlowData(table, table->cellAt(2, startCol), element->feed(), false);

                insertText(table->cellAt(1, startCol+1), tr("Permeate"));
                insertFlowData(table, table->cellAt(2, startCol+1), element->permeate(), false);

                insertText(table->cellAt(1, startCol+2), tr("Concentrate"));
                insertFlowData(table, table->cellAt(2, startCol+2), element->concentrate(), false);

                ++elementsTableNum;
                --elementsRestNum;
            }
        }

    }


    restoreFormat();
    moveCursorToEnd();
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

void ROReportBuilder::insertFlowData(QTextTable *table, const QTextTableCell &startCell, ROFlow *flow, bool ions) {
    int row = startCell.row();
    int col = startCell.column();

    ROSystem* sys = case_()->sys();
    QVector<int> filledIons = sys->filledIons();

    insertText(table->cellAt(row++, col), flow->rate());
    insertText(table->cellAt(row++, col), flow->osmoticPressure());
    insertText(table->cellAt(row++, col), flow->pressure());
    insertText(table->cellAt(row++, col), flow->solutes()->totalValueMgl());

    if (ions) {
        Q_FOREACH(const int& si, filledIons)
            insertText(table->cellAt(row++, col), flow->solutes()->mgl(si));
    }
}

QTextTable *ROReportBuilder::insertDataTable(int rows, int datacols, qreal datacolLength) {
    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_None);
    QVector<QTextLength> constraints;
    constraints << QTextLength(QTextLength::FixedLength, 100); // header column
    constraints << QTextLength(QTextLength::FixedLength, 50); // units column

    //int dataColNum = 1 + case_()->sys()->passCount(); // 1 - System
    for (int dcIdx=0; dcIdx<datacols; ++dcIdx)
        constraints << QTextLength(QTextLength::FixedLength, datacolLength); // data columns // 7 - MAX COLUMNS
    tableFormat.setColumnWidthConstraints(constraints);

    //int rows = 7;
    int columns = 2 + datacols; // PARAMS, UNITS

    return _cursor.insertTable(rows, columns, tableFormat);
}

void ROReportBuilder::insertLineBreak(int n) {
    for (int i=0; i<n; ++i)
        //_cursor.insertHtml("<br>");
        _cursor.insertBlock();
}


ROReportBuilder::~ROReportBuilder() {

}

void ROReportBuilder::setCase(ROCase *const case_) { _case = case_; caseChanged(); }
ROCase *const ROReportBuilder::case_() const { return _case; }



void ROReportBuilder::insertCostsTotalsDetails() {
    ROCosts* costs = case_()->costs();

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
    ROCosts* costs = case_()->costs();

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

    insertText(table->cellAt(2, 2), case_()->sys()->elementsCount());
    insertText(table->cellAt(2, 3), costs->membranesYearChangeFrequency());
    insertText(table->cellAt(2, 4), costs->membranesYearCount());
    insertText(table->cellAt(2, 5), costs->membraneCost());
    insertText(table->cellAt(2, 6), costs->membranesYearCost());

    insertText(table->cellAt(3, 1), tr("Energy by 1 m³ (kW/h)"));
    insertText(table->cellAt(3, 2), tr("Energy by system (kW/h)"));
    insertText(table->cellAt(3, 4), tr("Energy of a year"));
    insertText(table->cellAt(3, 5), tr("Energy cost (rub/(kW/h))"));

    insertText(table->cellAt(4, 1), case_()->sys()->specificEnergy());
    insertText(table->cellAt(4, 2), case_()->sys()->power());
    insertText(table->cellAt(4, 4), costs->electricEnergyYearAmount());
    insertText(table->cellAt(4, 5), costs->electricEnergyCost());
    insertText(table->cellAt(4, 6), costs->electricEnergyYearCost());

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertCostsChemistryDetails() {
    ROCosts* costs = case_()->costs();

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
    ROCosts* costs = case_()->costs();

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

    insertText(table->cellAt(1, 1), case_()->sys()->feed()->rate());
    insertText(table->cellAt(1, 2), costs->rawWaterYearAmount());
    insertText(table->cellAt(1, 3), costs->rawWaterCost());
    insertText(table->cellAt(1, 4), costs->rawWaterYearCost());

    insertText(table->cellAt(2, 1), case_()->sys()->concentrate()->rate());
    insertText(table->cellAt(2, 2), costs->concentrateDropYearAmount());
    insertText(table->cellAt(2, 3), costs->concentrateDropCost());
    insertText(table->cellAt(2, 4), costs->concentrateDropYearCost());

    moveCursorToEnd();
    restoreFormat();
}

void ROReportBuilder::insertCostsMaintenanceDetails() {
    ROCosts* costs = case_()->costs();

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
