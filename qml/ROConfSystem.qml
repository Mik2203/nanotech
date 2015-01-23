import QtQuick 1.1
import ROUnits 1.0
import "widgets" as ROWidgets

Rectangle {
    color: "transparent"
    ROWidgets.ScrollArea {
        anchors.fill: parent
        frame: false


        Row {
            spacing: 50

            Column {

                Column { // system params
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Item {
                        anchors.left: parent.left
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
                            anchors.rightMargin: parent.height-3
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("year(s)", "", sys.elementLifetime)
                            font.italic: true
                            horizontalAlignment: Text.AlignLeft
                            width: 30
                        }
                    }

                    Column {  // read-only system params
                        anchors.left: parent.left
//                        anchors.leftMargin: 10
                        anchors.right: parent.right

                        Item { // SYSTEM FLOW FACTOR ROW
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 20

                            Text {
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                font.italic: true
                                text: app.translator.emptyString + qsTr("Flow factor: ")
                            }

                            Text {
                                id: sysFlowFactor
                                anchors.right: parent.right
                                anchors.rightMargin: 5 + 30 + parent.height-3
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: TextInput.AlignRight
                                width: 50
                                font.italic: true
                                text: sys.flowFactor.toFixed(2)
                            }
                        }

                        Item {
                            id: systemRawWaterRow
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 20

                            Text {
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                text: app.translator.emptyString + qsTr("System raw water:")
                                font.italic: true
                            }

                            Text {
                                anchors.right: systemRawWaterUnits.left
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                height: parent.height-3
                                width: 50
                                text: app.units.convertFlowUnits(sys.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2)
                                font.italic: true
                            }

                            Text {
                                id: systemRawWaterUnits
                                anchors.right: parent.right
                                anchors.rightMargin: parent.height-3
                                anchors.verticalCenter: parent.verticalCenter
                                text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                                font.italic: true
                                horizontalAlignment: Text.AlignLeft
                                width: 30
                            }
                        }

                        Item {
                            id: totalElementsRow
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 20

                            Text {
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                text: app.translator.emptyString + qsTr("Total elements in system:")
                                font.italic: true
                            }

                            Text {
                                anchors.right: parent.right
                                anchors.rightMargin: 5 + 30 + parent.height-3
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                height: parent.height-3
                                width: 50
                                text: sys.elementsCount
                                font.italic: true
                            }
                        }

                        Item {
                            id: systemRecoveryRow
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 20

                            Text {
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                text: app.translator.emptyString + qsTr("System recovery:")
                                font.italic: true
                            }

                            Text {
                                anchors.right: systemRecoveryUnits.left
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                height: parent.height-3
                                width: 50
                                text: (sys.recovery * 100).toFixed(2)
                                font.italic: true
                            }

                            Text {
                                id: systemRecoveryUnits
                                anchors.right: parent.right
                                anchors.rightMargin: parent.height-3
                                anchors.verticalCenter: parent.verticalCenter
                                text: "%"
                                font.italic: true
                                horizontalAlignment: Text.AlignLeft
                                width: 30
                            }
                        }
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
//                        radius: 3
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
                        onClicked: mainWindow.popupWindow(Qt.createComponent("ROGenericSchemesTreeView.qml"), app.translator.emptyString + qsTr("Choose generic scheme"), mainWindow, undefined, undefined, true)
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
