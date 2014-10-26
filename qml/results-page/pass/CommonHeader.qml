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
}
