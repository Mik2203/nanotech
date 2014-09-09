import QtQuick 1.1
import "widgets" as ROWidgets
import ROFlow 1.0
import ROFeed 1.0
import ROSolutes 1.0
import ROScalingElement 1.0

Item {  // нужен внешний элемент, т.к. Connections не может быть внутри TabFrame
    Connections {
        target: sys.scalingElement
        onAdjustmentChanged: tabContainer.current = sys.scalingElement.adjustment  // rebind
    }


    ROWidgets.TabFrame {
        id: tabContainer

        anchors.fill: parent

        frame: false

        onCurrentChanged: sys.scalingElement.adjustment = current
        current: sys.scalingElement.adjustment

        ROWidgets.Tab {
            id: noAdjustmentTab
            anchors.margins: 10
            title: app.translator.emptyString + qsTr("No adjustment")
            ROWidgets.ScrollArea {
                anchors.fill: parent
                ScalingTable {
                    width: noAdjustmentTab.width-20 // -20 to hide scroll
                }
            }
        }

        ROWidgets.Tab {
            id: naSofteringTab
            anchors.margins: 10
            title: app.translator.emptyString + qsTr("Na-softering")

            ROWidgets.ScrollArea {
                anchors.fill: parent

                Column {
                    width: naSofteringTab.width-20

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 20

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("Ca Leakage:")
                        }

                        ROWidgets.DoubleInput {
                            id: caLeakageInput
                            anchors.right: caLeakageUnits.left
                            anchors.rightMargin: 5
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: TextInput.AlignRight
                            height: parent.height-3
                            width: 50

                            value: sys.scalingElement.caLeakage
                            onInputChanged: sys.scalingElement.caLeakage = changedValue

                        }

                        Text {
                            id: caLeakageUnits
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("mg/l")
                            font.italic: true
                            horizontalAlignment: Text.AlignLeft
                            width: 55
                        }
                    }

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 20

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("Mg Leakage:")
                        }

                        ROWidgets.DoubleInput {
                            id: mgLeakageInput
                            anchors.right: mgLeakageUnits.left
                            anchors.rightMargin: 5
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: TextInput.AlignRight
                            height: parent.height-3
                            width: 50

                            value: sys.scalingElement.mgLeakage
                            onInputChanged: sys.scalingElement.mgLeakage = changedValue
                        }

                        Text {
                            id: mgLeakageUnits
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("mg/l")
                            font.italic: true
                            horizontalAlignment: Text.AlignLeft
                            width: 55
                        }
                    }

                    ScalingTable {
                        width: naSofteringTab.width-20
                    }
                }
            }
        }

        ROWidgets.Tab {
            id: phAdjustmentTab
            anchors.margins: 10
            title: app.translator.emptyString + qsTr("pH-adjustment")

            ROWidgets.ScrollArea {
                anchors.fill: parent

                Column {
                    width: phAdjustmentTab.width-20

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 20

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("Dosing:")
                        }

                        ROWidgets.ComboBox {
                            id: dosingSelector
                            //property int idx: 0
                            anchors.right: parent.right
                            anchors.rightMargin: 60
                            anchors.verticalCenter: parent.verticalCenter
                            textVerticalAlignment: Text.AlignVCenter
                            width: 50
                            height: parent.height-3
                            model: [ "H2SO4", "HCl", "NaOH"] // TODO - сделать модель из С++ кода
                            selectedIndex: sys.scalingElement.dosingAcid
                            onSelect: sys.scalingElement.dosingAcid = selectIndex
                        }
                    }

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 20

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("Target pH:")
                        }

                        ROWidgets.DoubleInput {
                            id: targetPhInput
                            anchors.right: parent.right
                            anchors.rightMargin: 60
                            anchors.verticalCenter: parent.verticalCenter
                            height: parent.height-3
                            width: 50

                            value: sys.scalingElement.targetPh
                            onInputChanged: sys.scalingElement.targetPh = changedValue
                        }


                    }

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 20

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("Reagent concentration:")
                        }

                        ROWidgets.DoubleInput {
                            id: acidConcentrationInput
                            anchors.right: parent.right
                            anchors.rightMargin: 60
                            anchors.verticalCenter: parent.verticalCenter
                            height: parent.height-3
                            width: 50

                            value: sys.scalingElement.acidConcentration * 100.0
                            onInputChanged: sys.scalingElement.acidConcentration = changedValue / 100.0
                        }

                        Text {
                            id: acidConcentrationUnits
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: app.translator.emptyString + qsTr("%")
                            font.italic: true
                            horizontalAlignment: Text.AlignLeft
                            width: 55
                        }
                    }

                    ScalingTable {
                        width: phAdjustmentTab.width-20
                    }
                }
            }
        }
    }
}


