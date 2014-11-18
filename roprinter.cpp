#include "roprinter.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include "roapplication.h"

ROPrinter::ROPrinter(ROProject* proj, QObject *parent) :
    QObject(parent),
    project(proj)  // TODO
{
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
//        printer.setPageMargins(40, 40, 40, 40, QPrinter::Point);
        QTextDocument * const doc = _reportBuilder->build(pageRect.size());
        doc->documentLayout()->setPaintDevice(&printer);
        doc->setPageSize(printer.pageRect().size());


//        doc->print(&printer);





        QRect innerRect = printer.pageRect();

        int headerHeight = innerRect.height() * 0.04;
        int footerHeight = innerRect.height() * 0.04;

        innerRect.setTop(innerRect.top() + headerHeight);
        innerRect.setBottom(innerRect.bottom() - footerHeight);
        QSize pageMargins(innerRect.x(), innerRect.y());
        QRect contentRect = QRect(QPoint(0,0), doc->size().toSize());
        QRect currentRect = QRect(QPoint(0,0), innerRect.size());
        QPainter painter(&printer);
        int count = 0;
        painter.save();
        painter.translate(pageMargins.width(), headerHeight);
        while (currentRect.intersects(contentRect)) {
            doc->drawContents(&painter, currentRect);
            painter.setPen(QPen(Qt::red, 4));
            painter.drawRect(currentRect);
            count++;
            painter.restore();
            painter.setPen(QPen(Qt::blue, 1));
            painter.drawRect(currentRect.x(), currentRect.top(), currentRect.width(), headerHeight);
            painter.drawText(0, 0, tr("Nanotech PRO 1.3.x - %1 by %2 (%3)")
                             .arg(project->info()->name().isEmpty() ? tr("<unnamed>") : project->info()->name())
                             .arg(project->info()->author().isEmpty() ? tr("<author not specified>") : project->info()->author())
                             .arg(project->info()->company().isEmpty()? tr("<company not specified>") : project->info()->company()));
            painter.drawText(currentRect.width() * 0.8, 0, tr("%1    page %2")
                             .arg(QDate::currentDate().toString(Qt::ISODate))
                             .arg(count));
            painter.drawText(0, currentRect.bottom(), currentRect.width(), footerHeight,
                             Qt::AlignLeft,
                             tr("Please serve the results of the calculation using the program Nanotech PRO as guideline only.\n\"RM Nanotech\" is not responsible for any problems occurring during operation of the designed system."));
            painter.setPen(QPen(Qt::blue, 1));
            painter.drawRect(currentRect.x(), currentRect.bottom(), currentRect.width(), footerHeight);
            painter.save();

            currentRect.translate(0, currentRect.height());
            painter.translate(pageMargins.width(), -currentRect.height() * count + headerHeight);
            if (currentRect.intersects(contentRect))
                printer.newPage();
        }
        painter.restore();
        painter.end();
//        return 0;






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
