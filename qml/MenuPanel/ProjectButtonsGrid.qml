// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "../widgets" as ROWidgets

Row {
    spacing: 5

    ROWidgets.Button {
        id: newProjectButton
        width: 30
        height: 30
        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"
        Image {
            source: parent.hovered ? "../../images/project-new-hover.png" : "../../images/project-new.png"
            anchors.fill: parent
            smooth: true
        }

        onClicked: app.projectManager.newProject()

        active: (!sysSS.calculating)

        tooltip: app.translator.emptyString + qsTr("New project")
    }

    ROWidgets.Button {
        id: openProjectButton
        width: 30
        height: 30
        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"
        Image {
            source: parent.hovered ? "../../images/project-open-hover.png" : "../../images/project-open.png"
            anchors.fill: parent
            smooth: true
        }

        active: (!sysSS.calculating)

        onClicked: app.projectManager.openProject()

        tooltip: app.translator.emptyString + qsTr("Open project")
    }

    ROWidgets.Button {
        id: saveProjectButton
        width: 30
        height: 30
        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"
        Image {
            source: parent.hovered ? "../../images/project-save-hover.png" : "../../images/project-save.png"
            anchors.fill: parent
            smooth: true
        }

        onClicked: app.projectManager.saveProject()

        tooltip: app.translator.emptyString + qsTr("Save project")
    }

    ROWidgets.Button {
        id: saveProjectAsButton
        width: 30
        height: 30
        enabledColor: "white"
        disabledColor: "lightgrey"
        hoveredColor: "white"
        toggledColor: "white"
        Image {
            source: parent.hovered ? "../../images/project-save-as-hover.png" : "../../images/project-save-as.png"
            anchors.fill: parent
            smooth: true
        }

        onClicked: app.projectManager.saveProjectAs()

        tooltip: app.translator.emptyString + qsTr("Save project as")
    }

    ROWidgets.Button {
        id: projectInfoButton
        width: 30
        height: 30
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
        onClicked: mainWindow.popup(Qt.createComponent("../ROProjectInfoWindow.qml"), projectInfoButton, 10 + projectInfoButton.width, 0)

        tooltip: app.translator.emptyString + qsTr("Project info")
    }


}
