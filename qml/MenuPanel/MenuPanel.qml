// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import ROSystemSolveScheduler 1.0
import "../widgets" as ROWidgets
import "../"

Item {


    ProjectButtonsGrid {
        id: projectButtonsGrid
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    ParametersBar {
        id: caseBar
        label: app.translator.emptyString + qsTr("Case:")
        anchors.left: projectButtonsGrid.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        deletable: !sysSS.calculating

        elementsCount: proj.caseCount
        elementsCountMax: proj.MAX_CASES_COUNT
        elementsCountMin: proj.MIN_CASES_COUNT
        selectedIndex: proj.caseIndex(case_)
        onAdd: proj.addCase(index)
        onRemove: { proj.removeCase(index); /*passConf.pass = system.pass(selectedIndex)*/ }
        onSelect: { case_ = proj.case_(index) /*proj.curCaseIndex = index; selectedIndex = proj.curCaseIndex; passConf.pass = system.pass(selectedIndex)*/ }

    }

    ROWidgets.Button {
        id: calculateButton
        text: sysSS.calculating ? app.translator.emptyString + qsTr("Calculating") : app.translator.emptyString + qsTr("Calculate")
        anchors.right: unitsGrid.left
        width: 150
        height: unitsGrid.height
        textFont.bold: true
        textFont.pixelSize: 16
        anchors.verticalCenter: parent.verticalCenter
        onClicked: sysSS.startSolving();
        active: ((!sysSS.calculated && !sysSS.calculating && !sysC.hasAnyCriticalWarnings) || (sysSS.calculated && sysSS.needUpdate))

    }

    UnitsGrid {
        id: unitsGrid
        anchors.right: languageSelector.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 5
    }

    ROWidgets.ComboBox {
        id: languageSelector
        anchors.right: helpButton.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 5
        height: 20
        model: [ "EN", "RU"] // TODO ?
        selectedIndex: app.translator.currentLanguage
        onSelect: app.translator.currentLanguage = selectIndex
    }

    ROWidgets.Button {
        id: helpButton
        anchors.right: aboutButton.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 5
        width: 30
        height: 30
        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"
        Image {
            source: parent.hovered ? "../../images/help-hover.png" : "../../images/help.png"
            anchors.fill: parent
            anchors.margins: 1
            smooth: true
        }

        onClicked: app.help()
        tooltip: app.translator.emptyString + qsTr("Help me!")
    }

    ROWidgets.Button {
        id: aboutButton
        width: 30
        height: 30
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 10
        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"
        Image {
            source: parent.hovered ? "../../images/project-info-hover.png" : "../../images/project-info.png"
            anchors.fill: parent
            anchors.margins: 2
            smooth: true
        }

        onClicked: {
            mainWindow.popupWindow(Qt.createComponent("../AboutWindow.qml"), app.translator.emptyString + qsTr("About"), mainWindow, undefined, undefined, true)
        }

        tooltip: app.translator.emptyString + qsTr("About")
    }
}
