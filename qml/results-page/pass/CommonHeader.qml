import QtQuick 1.1

import ROUnits 1.0
import ROPass 1.0


import "../util"

Column {
    property int recycleCount: sys.passCount-1

    SectionTitle { text: "Common" }

    Row {
        Column {
            TableRowTitle { title: app.translator.emptyString + qsTr("Power") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Specific energy") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Active Area") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Total elements count") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Avg. Flux") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Average stages recovery") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Raw water flow") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Blending") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Feed flow") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Self recycle") }

            Repeater {
                model: recycleCount

                TableRowTitle { title: app.translator.emptyString + qsTr("Recycle to pass %1").arg(index+1) }
            }

            TableRowTitle { title: app.translator.emptyString + qsTr("Stage 1 feed flow") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Total product") }
        }

        Column {
            TableRowUnits { units: app.translator.emptyString + qsTr("kW") }
            TableRowUnits { units: app.translator.emptyString + qsTr("kWh/m³") }
            TableRowUnits { units: app.translator.emptyString + unitsText.areaUnitText(app.units.areaUnits) }
            TableRowUnits { units: "" }
            TableRowUnits { units: app.translator.emptyString + unitsText.fluxUnitText(app.units.fluxUnits) }
            TableRowUnits { units: "%" }
            TableRowUnits { units: "%" }
            TableRowUnits { units: "%" }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }

            Repeater {
                model: recycleCount

                TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            }

            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        }
    }



//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Feed TDS") }
//        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
//        TableRowValue { value: pass.feed.solutes.totalValueMgl }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Feed pressure") }
//        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
//        TableRowValue { value: app.units.convertPressureUnits(pass.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Feed osmotic pressure") }
//        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
//        TableRowValue { value: app.units.convertPressureUnits(pass.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Feed pH") }
//        TableRowUnits { units: "" }
//        TableRowValue { value: pass.feed.pH }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate flow") }
//        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
//        TableRowValue { value: app.units.convertFlowUnits(pass.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate TDS") }
//        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
//        TableRowValue { value: pass.permeate.solutes.totalValueMgl }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pressure") }
//        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
//        TableRowValue { value: app.units.convertPressureUnits(pass.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate osmotic pressure") }
//        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
//        TableRowValue { value: app.units.convertPressureUnits(pass.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
//    }

//    Row {
//        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pH") }
//        TableRowUnits { units: "" }
//        TableRowValue { value: pass.permeate.pH }
//    }
}
