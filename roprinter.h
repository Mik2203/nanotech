#ifndef ROPRINTER_H
#define ROPRINTER_H

#include <QObject>

#include "roreportbuilder.h"

// управление выводом на печать и в PDF файл.

class ROPrinter : public QObject {
    Q_OBJECT
    Q_ENUMS(PrinterDevice)

    Q_PROPERTY(ROReportBuilder* reportBuilder READ reportBuilder CONSTANT)

public:
    enum PrinterDevice { NativeDevice, PDFDevice };
    explicit ROPrinter();
    ~ROPrinter();

    ROReportBuilder* const reportBuilder() const;
    
signals:
    
public slots:
    void print(ROCase * case_, PrinterDevice device);
    void printCosts(ROCase * case_, PrinterDevice device);

private:
    ROReportBuilder* _reportBuilder;

//    QPrinter setupPrinter(PrinterDevice device);
    void printReport(QPrinter& printer, QTextDocument * const doc);
    
};

#endif // ROPRINTER_H
