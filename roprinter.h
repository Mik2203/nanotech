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
    explicit ROPrinter(ROProject* proj, QObject *parent = 0);
    ROPrinter() {}; // for QML
    ~ROPrinter();

    ROReportBuilder* const reportBuilder() const;
    
signals:
    
public slots:
    void print(PrinterDevice device);
    void printPDF();
    void printNative();
    void printCosts(PrinterDevice device);
    void printCostsPDF();
    void printCostsNative();

private:
    ROReportBuilder* _reportBuilder;
    
};

#endif // ROPRINTER_H
