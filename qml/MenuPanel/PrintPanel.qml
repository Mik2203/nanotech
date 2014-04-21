// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "../widgets" as ROWidgets
import ROPrinter 1.0

Row {
    spacing: 5
    signal print_(int device)

    Connections {
        target: mainWindow
        onCase_Changed: app.printer.reportBuilder.case_ = mainWindow.case_
    }

    ROWidgets.Button {
        id: printNativeButton
        width: 30
        height: 30

        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"

        Image {
            source: parent.hovered ? "../../images/print-native-hover.png" : "../../images/print-native.png"
            anchors.fill: parent
            smooth: true
        }

        active: (!sysSS.calculating)

        onClicked: print_(ROPrinter.NativeDevice); //app.printNative()
        tooltip: app.translator.emptyString + qsTr("Print results")
    }


    ROWidgets.Button {
        id: printPdfButton
        width: 30
        height: 30

        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"

        Image {
            source: parent.hovered ? "../../images/print-pdf-hover.png" : "../../images/print-pdf.png"
            anchors.fill: parent
            smooth: true
        }

        active: (!sysSS.calculating)

        onClicked: print_(ROPrinter.PDFDevice); //app.printPDF()
        tooltip: app.translator.emptyString + qsTr("Save results as PDF")
    }

}
