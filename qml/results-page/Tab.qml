// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "../widgets" as ROWidgets
import "../MenuPanel"
import "../"

Rectangle {
    color: "transparent"

    PrintPanel {
        id: printButtons
        //z: 2 // UNCOMMENT TO BE ON TOP
        visible: sysSS.solved
        anchors.right:  parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        onPrint_: app.printer.print(device)
    }

    ROWidgets.ScrollArea {
        anchors.fill: parent
        frame: false

        Component { id: nullWidthStub; Item {width: 0; height: 0}   }

        Column {
            id: resultData

            spacing: 20
            Warnings { }

            Loader {
                id: resultsScheme
                Component {
                    id: resultsSchemeComponent;

                    ROSchemeSystem {
                        editable: false;
                        showWarnings: true;
                    }
                }
                sourceComponent: sysSS.solved ? resultsSchemeComponent : nullWidthStub
                onLoaded: schemeCapturer.item = item
            }




            Loader {
                Component { id: resultsComponent; Data { } }
                sourceComponent: sysSS.solved ? resultsComponent : nullWidthStub
            }
        }
    }
}
