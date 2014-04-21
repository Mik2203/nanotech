import QtQuick 1.1
import "widgets" as ROWidgets

Rectangle {
    color: "transparent"
    ROWidgets.ScrollArea {
        anchors.fill: parent
        frame: false

        Connections { target: mainWindow; onSysChanged: selectedPass = sys.firstPass }
        Connections { target: sys; onPassCountChanged: { if (sys.passIndex(selectedPass) == -1) selectedPass = sys.firstPass } } // pass is undefined, so reselect


        Row {
            spacing: 50

            Item {
                id: passContainer
                width: 300
                height: passConf.height + passBar.height / 2 + 30

                Rectangle {
                    id: border
                    border.color: "grey"
                    border.width: 1
                    color: "white"
                    radius: 3
                    anchors.fill: parent
                    anchors.topMargin: passBar.height / 2
                }

                ParametersBar {
                    id: passBar
                    z: 1
                    anchors.top: parent.top
                    //anchors.topMargin: -20
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    label: app.translator.emptyString + qsTr("Pass:")
                    elementsCount: sys.passCount
                    elementsCountMax: sys.MAX_PASSES_COUNT
                    elementsCountMin: sys.MIN_PASSES_COUNT
                    selectedIndex: sys.passIndex(selectedPass)
                    onAdd: sys.addPass(index)
                    onRemove: sys.removePass(index);
                    onSelect: selectedPass = sys.pass(index);
                }

                ROConfPass {
                    id: passConf
                    pass: selectedPass
                    anchors.top: passBar.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 15
                }

            }

            Column {
                Row {
                    anchors.left: parent.left
                    spacing: 10

                    ROWidgets.Button {
                        text: app.translator.emptyString + qsTr("Generic schemes")
                        onClicked: mainWindow.popupWindow(Qt.createComponent("ROGenericSchemesTreeView.qml"), app.translator.emptyString + qsTr("Choose generic scheme"), mainWindow, mainWindow.width/2 - 200, mainWindow.height/2 - 150)
                    }

                    ROWidgets.Button {
                        text: app.translator.emptyString + qsTr("Reset scheme")
                        onClicked: { sys.resetSystem(); sysC.reset(); }
                    }
                }

                ROSchemeSystem {}
            }
        }
    }
}
