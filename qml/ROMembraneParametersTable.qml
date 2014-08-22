// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import ROUnits 1.0
import "widgets" as ROWidgets

ROWidgets.SimpleListView {
    id: membraneModelView
    focus: true
    model: db.membranes
    //selectedIndex: stage.membraneIndex
    onSelect: stage.membraneId = model.get(index, "id")
    onClose: mainWindow.hidePopup()
    headerDelegate: Row {
//        height: 20
        anchors.left: parent.left
        anchors.leftMargin: 3
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Model"); font.bold: true }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Area\n(%1)").arg(unitsText.areaUnitText(app.units.areaUnits)); font.bold: true; width: 70 }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Productivity\n(%1)").arg(unitsText.flowUnitText(app.units.flowUnits)); font.bold: true; width: 70 }
        ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Rejection\n(%)"); font.bold: true; width: 70 }
        Column {
            width: test_params_header.width

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                clip: true
                font.bold: true
                elide: Text.ElideRight
                text: app.translator.emptyString + qsTr("Test conditions")
            }

            Row {
                id: test_params_header
                ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Soln. (%)"); font.bold: true; width: 80 }
                ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Pres. (%1)").arg(unitsText.pressureUnitText(app.units.pressureUnits)); font.bold: true; width: 70 }
                ROMembraneParametersCell { text: app.translator.emptyString + qsTr("Recov. (%)"); font.bold: true; width: 70 }
            }
        }
    }

    itemDelegate: ROWidgets.DefaultListViewDelegate {
        height: 20
        width: parametersRow.width
        Row {
            id: parametersRow
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 5
            ROMembraneParametersCell { text: model.series + " " + model.model_  }
            ROMembraneParametersCell { text: app.units.convertAreaUnits(model.area, ROUnits.DEFAULT_AREA_UNITS, app.units.areaUnits).toFixed(2); width: 70 }
            ROMembraneParametersCell { text: app.units.convertFlowUnits(model.productivity, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2); width: 70 }
            ROMembraneParametersCell { text: model.rejection; width: 70 }
            ROMembraneParametersCell { text: model.test_solution_name + " " + model.test_solution_concentration + "%"; width: 80 }
            ROMembraneParametersCell { text: app.units.convertPressureUnits(model.test_pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits).toFixed(2); width: 70 }
            ROMembraneParametersCell { text: model.test_recovery * 100; width: 70 }
        }

        color: stage.membraneId === model.id ? "#018CD1" :
                                               (index === listView.currentIndex ? "#caebff" : "white")
    }
}
//}
