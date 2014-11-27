#include "roprinter.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include "roapplication.h"

ROPrinter::ROPrinter()
{
    _reportBuilder = new ROReportBuilder();
}

ROPrinter::~ROPrinter() {
    delete _reportBuilder;
}

ROReportBuilder* const ROPrinter::reportBuilder() const { return _reportBuilder; }

void ROPrinter::print(ROCase * const case_, ROPrinter::PrinterDevice device) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);

    switch (device) {
    case NativeDevice: {
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle(tr("Print project results"));
        if (dialog->exec() != QDialog::Accepted)
            return;
        delete dialog;
        break;
    }
    case PDFDevice: {
        QString filename = QFileDialog::getSaveFileName(0, tr("Save PDF File"), QString(), "PDF (*.pdf)");
        if (filename.isEmpty())
            return;
        if(QFileInfo(filename).suffix().isEmpty())
            filename.append(".pdf");
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        break;
    }
    }

#ifndef QT_NO_CURSOR
    ROApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    printReport(printer, _reportBuilder->build(case_, printer.pageRect(QPrinter::Point).size()));

#ifndef QT_NO_CURSOR
    ROApplication::restoreOverrideCursor();
#endif
}

void ROPrinter::printCosts(ROCase * case_, ROPrinter::PrinterDevice device) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);

    switch (device) {
    case NativeDevice: {
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle(tr("Print project results"));
        if (dialog->exec() != QDialog::Accepted)
            return;
        delete dialog;
        break;
    }
    case PDFDevice: {
        QString filename = QFileDialog::getSaveFileName(0, tr("Save PDF File"), QString(), "PDF (*.pdf)");
        if (filename.isEmpty())
            return;
        if(QFileInfo(filename).suffix().isEmpty())
            filename.append(".pdf");
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        break;
    }
    }


#ifndef QT_NO_CURSOR
    ROApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    printReport(printer, _reportBuilder->buildCosts(case_, printer.pageRect(QPrinter::Point).size()));

#ifndef QT_NO_CURSOR
    ROApplication::restoreOverrideCursor();
#endif
}


//QPrinter ROPrinter::setupPrinter(ROPrinter::PrinterDevice device)
//{
//    QPrinter printer(QPrinter::HighResolution);
//    printer.setPaperSize(QPrinter::A4);

//    QPrintDialog *dialog = new QPrintDialog(&printer);
//    dialog->setWindowTitle(tr("Print project results"));
////    if (dialog->exec() != QDialog::Accepted)
////        return;
//    return printer;
//}

void ROPrinter::printReport(QPrinter &printer, QTextDocument * const doc)
{
    qreal headerFooterMargin = printer.pageRect(QPrinter::Point).height() * 0.04;
    printer.setPageMargins(20, 20 + headerFooterMargin, 20, 20 + headerFooterMargin, QPrinter::Point);
    QRectF pageRect = printer.pageRect(QPrinter::Point);
    doc->documentLayout()->setPaintDevice(&printer);
    doc->setPageSize(printer.pageRect().size());


    QRect innerRect = printer.pageRect();

    int headerHeight = innerRect.height() * 0.04;

    QRect contentRect = QRect(QPoint(0,0), doc->size().toSize());
    QRect currentRect = QRect(QPoint(0,0), innerRect.size());
    QPainter painter(&printer);
    int count = 0;
    painter.save();
    while (currentRect.intersects(contentRect)) {
        doc->drawContents(&painter, currentRect);
        count++;
        painter.restore();
        // Draw header
        painter.drawText(0, -headerHeight / 2, tr("Nanotech PRO 1.3.x - %1 by %2 (%3)")
                         .arg(roApp->projectManager()->proj()->info()->name().isEmpty() ? tr("<unnamed>") : roApp->projectManager()->proj()->info()->name())
                         .arg(roApp->projectManager()->proj()->info()->author().isEmpty() ? tr("<author not specified>") : roApp->projectManager()->proj()->info()->author())
                         .arg(roApp->projectManager()->proj()->info()->company().isEmpty()? tr("<company not specified>") : roApp->projectManager()->proj()->info()->company()));
        painter.drawText(currentRect.width() * 0.8, -headerHeight / 2, tr("%1    page %2")
                         .arg(QDate::currentDate().toString(Qt::ISODate))
                         .arg(count));

        // Draw footer
        painter.drawText(0, printer.pageRect().bottom() - headerHeight * 1.5, currentRect.width(), headerHeight,
                         Qt::AlignLeft,
                         tr("Please serve the results of the calculation using the program Nanotech PRO as guideline only.\n"
                            "\"RM Nanotech\" is not responsible for any problems occurring during operation of the designed system."));
        painter.save();

        currentRect.translate(0, currentRect.height());
        painter.translate(0, -currentRect.height() * count);
        if (currentRect.intersects(contentRect))
            printer.newPage();
    }
    painter.restore();
    painter.end();
}
