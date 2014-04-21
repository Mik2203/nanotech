// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets

ROWidgets.SimpleListView {
    id: membraneModelView
    focus: true
    model: db.membraneAlternatives
    selectedIndex: stage.membraneIndex
    //        enableHorizontalScrollBar: true
    onSelect: stage.membraneIndex = db.membranes.indexById(db.membraneAlternatives.get(index, "id"))
    onClose: mainWindow.hidePopup()
    headerDelegate: Row {
        height: 20
        anchors.left: parent.left
        anchors.leftMargin: 3
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Nanotech"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Dow"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("CSM"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Hydranautics"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("TORAY"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("KOCH"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("DESAL/GE"); font.bold: true }
    }

    itemDelegate: ROWidgets.DefaultListViewDelegate {
        //id: cbItemDelegate
        height: 20
        width: parametersRow.width
        Row {
            id: parametersRow
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 3
            ROMembraneParametersCell { text: model.nanotech; font.bold: true }
            ROMembraneParametersCell { text: model.dow }
            ROMembraneParametersCell { text: model.csm }
            ROMembraneParametersCell { text: model.hydranautics }
            ROMembraneParametersCell { text: model.toray }
            ROMembraneParametersCell { text: model.koch }
            ROMembraneParametersCell { text: model.desalGE }
        }
    }
}
//}
