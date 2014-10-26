// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import ROSystem 1.0
//import ROSystemController 1.0
import ROStage 1.0
import ROPass 1.0

import ROSolutes 1.0
import ROSoluteModel 1.0
import ROWaterTypeModel 1.0
import ROUnits 1.0

import "../widgets" as ROWidgets

import "./system" as SystemResults
import "./pass" as PassResults
import "./stage" as StageResults
import "./element" as ElementResults

Column {
    spacing: 10
    property int _TITLE_WIDTH: 150
    property int _UNITS_WIDTH: 50
    property int _VALUE_WIDTH: 80
    property color _BORDER_COLOR: "grey"
    property int _ROW_HEIGHT: 20

    ROSoluteModel { id: _SOLUTE_MODEL } // to access static fields only

//    height: mainColumn.height
//    width: mainColumn.width

    SystemResults.Data {}
    PassResults.Data {}
    StageResults.Data {}
    ElementResults.Data {}
}
