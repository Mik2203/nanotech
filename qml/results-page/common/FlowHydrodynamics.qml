import QtQuick 1.1

import ROUnits 1.0

import "../util"

Row {
    property variant flows
    Column {
        // headers
        Item { // spacer
            height: _ROW_HEIGHT
            width: _TITLE_WIDTH + _UNITS_WIDTH
        }

        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Flow") }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        }
        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Pressure") }
            TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        }
        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Osmotic pressure") }
            TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        }
    }

    // data
    Repeater {
        model: flows
        Column {
            TableRowValue { value: modelData.title }
            TableRowValue { value: app.units.convertFlowUnits(modelData.flow.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
            TableRowValue { value: app.units.convertPressureUnits(modelData.flow.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
            TableRowValue { value: app.units.convertPressureUnits(modelData.flow.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
        }
    }
}
