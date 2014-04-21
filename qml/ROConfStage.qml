import QtQuick 1.1
import ROPass 1.0
import ROStage 1.0
import ROFlow 1.0
import ROUnits 1.0
import ROStageController 1.0
//import RODatabase 1.0

import "widgets" as ROWidgets

Item {
    id: panel
    property ROStage stage
    property ROStageController stageC: passC.stageC(stage)
    height: column_panel.height

    Column {
        id: column_panel
        width: panel.width
        Item { // STAGE VESSELS COUNT EDITOR ROW
            id: vesselsCountEditor
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Vessels count: ")
            }

            ROWidgets.TextInput {
                anchors.right: vesselsCountUnitsSpacer.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                validator: IntValidator {}
                height: parent.height-3
                width: 50
                value: stage.vesselCount
                onInputChanged: stage.vesselCount = changedValue
            }

            Item {
                id: vesselsCountUnitsSpacer
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 30 + parent.height-3
            }

            KeyNavigation.tab: elementsPerVesselCountEditor
        }

        Item { // STAGE ELEMENTS PER VESSELS EDITOR ROW
            id: elementsPerVesselCountEditor
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Elements per vessel: ")
            }

            ROWidgets.TextInput {
                anchors.right: elementsPerVesselCountUnitsSpacer.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                validator: IntValidator {}
                height: parent.height-3
                width: 50
                value: stage.elementsPerVesselCount
                onInputChanged: stage.elementsPerVesselCount = changedValue
            }

            Item {
                id: elementsPerVesselCountUnitsSpacer
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 30 + parent.height-3
            }

            KeyNavigation.tab: preStagePressureEditor
            KeyNavigation.backtab: vesselsCountEditor
        }

        Item { // STAGE PRESTAGE PRESSURE EDITOR ROW
            id: preStagePressureEditor
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Pre stage pressure: ")
            }

            ROWidgets.DoubleInput {
                anchors.right: stagePreStagePressureUnits.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                height: parent.height-3
                width: 50
                value: app.units.convertPressureUnits(stage.preStagePressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits)
                onInputChanged: stage.preStagePressure = app.units.convertPressureUnits(changedValue, app.units.pressureUnits, ROUnits.DEFAULT_PRESSURE_UNITS)
            }

            Text {
                id: stagePreStagePressureUnits
                anchors.right: stagePreStagePressureRightSpacer.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits)
                font.italic: true
                horizontalAlignment: Text.AlignLeft
                width: 30
            }

            Item {
                id: stagePreStagePressureRightSpacer
                anchors.right: parent.right
                width: parent.height-3
            }

            KeyNavigation.backtab: elementsPerVesselCountEditor
        }

        Item { // STAGE BACK PRESSURE EDITOR ROW
            id: backPressureEditor
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Back pressure: ")
            }

            ROWidgets.DoubleInput {
                anchors.right: stageBackPressureUnits.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                height: parent.height-3
                width: 50
                value: app.units.convertPressureUnits(stage.backPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits)
                onInputChanged: stage.backPressure = app.units.convertPressureUnits(changedValue, app.units.pressureUnits, ROUnits.DEFAULT_PRESSURE_UNITS)
            }

            Text {
                id: stageBackPressureUnits
                anchors.right: stageBackPressureRightSpacer.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits)
                font.italic: true
                horizontalAlignment: Text.AlignLeft
                width: 30
            }

            Item {
                id: stageBackPressureRightSpacer
                anchors.right: parent.right
                width: parent.height-3
            }

            KeyNavigation.backtab: elementsPerVesselCountEditor
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 18
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Membrane element:")
            }

            ROWidgets.ComboBox {
                id: membraneTypeEditor
                anchors.right: membraneDetails.left
                anchors.rightMargin: 7
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 2
                anchors.bottomMargin: 2
                textHorizontalAlignment: Text.AlignLeft
                textVerticalAlignment: Text.AlignVCenter
                width: 80
                //height: 25
                selectedText: model.get(selectedIndex, "series") + " " + model.get(selectedIndex, "model")

                itemDelegate: ROWidgets.DefaultListViewDelegate {
                    width: membraneTypeEditor.width
                    height: membraneTypeEditor.height
                    text: model.series + " " + model.model_
                }

                model: db.membranes
                onSelect: stage.membraneIndex = selectIndex
                selectedIndex: stage.membraneIndex
            }

            ROWidgets.Button {
                id: membraneDetails
                anchors.right: membraneChosenWarning.left
                anchors.rightMargin: 3
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 2
                anchors.bottomMargin: 2
                width: 20
                text: "..."

                // TODO
                onClicked: mainWindow.popupWindow(membraneModelContainer, app.translator.emptyString + qsTr("Choose membrane") ,mainWindow, mainWindow.width/2 - 200, mainWindow.height/2 - 150)

                Component {
                    id: membraneModelContainer

                    Column {
                        property bool parametersOption: !replaceButton.toggled
                        spacing: 10
                        width: tableLoader.width

                        ROWidgets.Button {
                            id: replaceButton
                            text: parametersOption ? app.translator.emptyString + qsTr("Elements alternatives") : app.translator.emptyString + qsTr("Elements parameters")
                            togglable: true
                        }

                        Loader {
                            id: tableLoader
                            source: parametersOption ? "ROMembraneParametersTable.qml" : "ROMembraneAlternativesTable.qml"
                        }
                    }
                }

            }

            ROWidgets.WarningPopup {
                id: membraneChosenWarning
                warning: stageC.membraneChosen
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }

        }
    }
    //}
}
