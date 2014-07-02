// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets

ROWidgets.SimpleListView {
    id: membraneModelView
    focus: true
    model: db.membranes
    //selectedIndex: stage.membraneIndex
    onSelect: stage.membraneIndex = index
    onClose: mainWindow.hidePopup()
    headerDelegate: Row {
//        height: 20
        anchors.left: parent.left
        anchors.leftMargin: 3
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Model"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Size\n(mm×mm)"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Area\n(m²)"); font.bold: true; width: 40 }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Productivity\n(m³/h)"); font.bold: true; width: 70 }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Rejection\n(%)"); font.bold: true; width: 60 }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Pressure\n(MPa)"); font.bold: true; width: 60 }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Test solution"); font.bold: true; width: 100 }
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
            ROMembraneParametersCell { text: model.series + " " + model.model_  }
            ROMembraneParametersCell { text: model.diameter + "×" + model.length }
            ROMembraneParametersCell { text: model.area; width: 40 }
            ROMembraneParametersCell { text: model.productivity; width: 70 }
            ROMembraneParametersCell { text: model.rejection; width: 60 }
            ROMembraneParametersCell { text: model.pressure; width: 60 }
            ROMembraneParametersCell { text: model.test_solution; width: 100 }
        }
    }
}
//}
