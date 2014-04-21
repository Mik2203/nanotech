// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import ROFlow 1.0
import "widgets" as ROWidgets
import ROWaterModel 1.0
import ROFeedController 1.0
import ROUnits 1.0

Item {
    id: container
    property ROFlow flow
    property ROFeedController feedC
    property bool editable: true

    Row {
        id: editButtonsRow
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 5

        ROWidgets.Button {
            id: balanceWaterButton
            active: container.editable
//            width: 50;
            height: 20;
            text: app.translator.emptyString + qsTr("Balance")
            onClicked: flow.solutes.adjust()
        }

        ROWidgets.Button {
            id: resetWaterButton
            active: container.editable
//            width: 50;
            height: 20;
            text: app.translator.emptyString + qsTr("Reset")
            onClicked: flow.solutes.reset()
        }
    }

    Item { // TEMPERATURE EDITOR ROW
        id: temperatureEditorRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: editButtonsRow.bottom
        anchors.topMargin: 5
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("Temperature: ")
        }

        ROWidgets.DoubleInput {
            id: temperatureInput
            anchors.right: temperatureUnits.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 60
            validator: DoubleValidator { bottom: 3.0; top: 45.0 }
            editable: container.editable //import ROUnits 1.0
            value: app.units.convertTemperatureUnits(flow.temperature, ROUnits.DEFAULT_TEMPERATURE_UNITS, app.units.temperatureUnits);//ROUnits.convertTemperatureUnits(flow.temperature, ROUnitConverterTemperature.Default, app.units.temperatureUnits)
            onInputChanged: flow.temperature = app.units.convertTemperatureUnits(changedValue, app.units.temperatureUnits, ROUnits.DEFAULT_TEMPERATURE_UNITS)
            KeyNavigation.tab: phInput
            //KeyNavigation.backtab:
        }

        Text {
            id: temperatureUnits
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + unitsText.temperatureUnitText(app.units.temperatureUnits)
            font.italic: true
            horizontalAlignment: Text.AlignLeft
            width: 15
        }

    }

    Item { // PH EDITOR ROW
        id: pHEditorRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: temperatureEditorRow.bottom
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("pH:")
        }

        ROWidgets.DoubleInput {
            id: phInput
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 60
            value: flow.pH
            validator: DoubleValidator { bottom: 5.0; top: 15.0 }
            editable: container.editable
            onInputChanged: flow.pH = changedValue
            KeyNavigation.backtab: temperatureInput
        }
    }

    //Item { height: 10; width: 1 } //SPACER
    Item {
        id: solutesHeader
        height: 20
        //width: parent.width
        anchors.top: pHEditorRow.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.rightMargin: 20
        anchors.right: parent.right
        Text { text: app.translator.emptyString + qsTr("Solute"); anchors.left: parent.left}
        Text { text: app.translator.emptyString + qsTr("mg/l"); font.italic: true; width: 70; anchors.right: meqlHeader.left; anchors.rightMargin: 5; horizontalAlignment: Text.AlignRight }
        Text { id: meqlHeader; text: app.translator.emptyString + qsTr("meq/l"); font.italic: true; width: 70; anchors.right: parent.right; /*anchors.rightMargin: parent.height+3;*/  horizontalAlignment: Text.AlignRight }
    }

    ROWidgets.WarningPopup {
        id: ionsFilledWarning
        warning: feedC.ionsFilled
        anchors.right: parent.right
        anchors.verticalCenter: solutesHeader.verticalCenter
    }

    ROWidgets.WarningPopup {
        id: ionsBalancedWarning
        warning: feedC.ionsBalanced
        anchors.right: ionsFilledWarning.right
        anchors.rightMargin: 2
        anchors.verticalCenter: solutesHeader.verticalCenter
    }

    ROWidgets.ScrollArea {
        id: solutesViewArea
        //height: 200
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: solutesHeader.bottom
        anchors.bottom: solutesManager.top
        anchors.bottomMargin: 5
        frame: false
        ROSolutesTableView {
            id: solutesView;
            width: container.width-20;
            editable: container.editable
        }
    }


    Item {
        id: solutesManager
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.top: solutesViewArea.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        height: 20
        //color: "pink"

        //Row {
        ROWidgets.TextInput {
            id: solutesName
            width: 50
            anchors.left: parent.left
            anchors.right: saveSolutesButton.left
            anchors.rightMargin: 5
            horizontalAlignment: TextInput.AlignRight
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            onInputChanged: value = changedValue
        }

        ROWidgets.Button {
            id: saveSolutesButton
            text: app.translator.emptyString + qsTr("Save")
            anchors.right: loadSolutesButton.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: 50
            onClicked: if (solutesName.value.length > 0) solutesIO.saveToDB(solutesName.value, currentSoluteModel) // 0 - user group id
        }

        ROWidgets.Button {
            id: loadSolutesButton
            text: app.translator.emptyString + qsTr("Load")
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: 50
            active: editable

            onClicked: mainWindow.popupWindow(Qt.createComponent("ROSolutesTreeView.qml"), app.translator.emptyString + qsTr("Choose water"), mainWindow, mainWindow.width/2 - 200, mainWindow.height/2 - 150)

        }
    }
}
