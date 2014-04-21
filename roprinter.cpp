#include "roprinter.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include "roapplication.h"

ROPrinter::ROPrinter(ROProject* proj, QObject *parent) :
    QObject(parent) {
    _reportBuilder = new ROReportBuilder(proj);
}

ROPrinter::~ROPrinter() {
    delete _reportBuilder;
}

ROReportBuilder* const ROPrinter::reportBuilder() const { return _reportBuilder; }

void ROPrinter::print(ROPrinter::PrinterDevice device) {
    switch (device) {
    case NativeDevice: printNative(); break;
    case PDFDevice: printPDF(); break;
    }
}


// TODO удалить дублирование кода
void ROPrinter::printPDF() {
    QString filename = QFileDialog::getSaveFileName(0, tr("Save PDF File"), QString(), "PDF (*.pdf)");
    if(!filename.isEmpty()) {
#ifndef QT_NO_CURSOR
    ROApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        if(QFileInfo(filename).suffix().isEmpty()) filename.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        printer.setPaperSize(QPrinter::A4);
        QRectF pageRect = printer.pageRect(QPrinter::Point);
        printer.setPageMargins(40, 40, 40, 40, QPrinter::Point);
        const QTextDocument* doc = _reportBuilder->build(pageRect.size());
        doc->print(&printer);
#ifndef QT_NO_CURSOR
    ROApplication::restoreOverrideCursor();
#endif
    }
}

void ROPrinter::printNative() {
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle(tr("Print project results"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    QRectF pageRect = printer.pageRect(QPrinter::Point);
    printer.setPageMargins(40, 40, 40, 40, QPrinter::Point);
    const QTextDocument* doc = _reportBuilder->build(pageRect.size());
    doc->print(&printer);
    delete dialog;
}

void ROPrinter::printCosts(ROPrinter::PrinterDevice device) {
    switch (device) {
    case NativeDevice: printCostsNative(); break;
    case PDFDevice: printCostsPDF(); break;
    }
}

void ROPrinter::printCostsPDF() {
    QString filename = QFileDialog::getSaveFileName(0, tr("Save PDF File"), QString(), "PDF (*.pdf)");
    if(!filename.isEmpty()) {
#ifndef QT_NO_CURSOR
    ROApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        if(QFileInfo(filename).suffix().isEmpty()) filename.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        printer.setPaperSize(QPrinter::A4);
        QRectF pageRect = printer.pageRect(QPrinter::Point);
        printer.setPageMargins(40, 40, 40, 40, QPrinter::Point);
        const QTextDocument* doc = _reportBuilder->buildCosts(pageRect.size());
        doc->print(&printer);
#ifndef QT_NO_CURSOR
    ROApplication::restoreOverrideCursor();
#endif
    }
}

void ROPrinter::printCostsNative() {
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle(tr("Print project results"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    QRectF pageRect = printer.pageRect(QPrinter::Point);
    printer.setPageMargins(40, 40, 40, 40, QPrinter::Point);
    const QTextDocument* doc = _reportBuilder->buildCosts(pageRect.size());
    doc->print(&printer);
    delete dialog;
}
