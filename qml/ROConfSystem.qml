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

            Column {

                Item {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.right: parent.right
                    height: 18

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: app.translator.emptyString + qsTr("Element lifetime:")
                    }

                    ROWidgets.ComboBox {
                        id: elementLifetimeEditor
                        anchors.right: elementLifetimeUnits.left
                        anchors.rightMargin: 5
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.topMargin: 2
                        anchors.bottomMargin: 2
                        textHorizontalAlignment: Text.AlignLeft
                        textVerticalAlignment: Text.AlignVCenter
                        width: 40
                        //height: 25
//                        selectedText: sys.lifetime
                        selectedIndex: sys.elementLifetime-1
                        model: [1,2,3,4,5] // TODO from sys consts
                        onSelect: sys.elementLifetime = selectIndex+1
                    }

                    Text {
                        id: elementLifetimeUnits
                        anchors.right: parent.right
                        anchors.rightMargin: 32
                        anchors.verticalCenter: parent.verticalCenter
                        text: app.translator.emptyString + qsTr("years")
                        font.italic: true
                        horizontalAlignment: Text.AlignLeft
                        width: 30
                    }
                }


                Item {
                    id: passContainer
                    width: 300 // TODO why 300?
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
