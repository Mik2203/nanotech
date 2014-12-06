import QtQuick 1.1

import ROUnits 1.0


import "../util"

Column {
    SectionTitle { text: app.translator.emptyString + qsTr("Common") }

    Row {
        Column {
            id: titles
            TableRowTitle { title: app.translator.emptyString + qsTr("Membrane element") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Total elements count") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Avg. Flux") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Feed flow") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Feed TDS") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Feed pressure") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Permeate flow") }
            TableRowTitle { title: app.translator.emptyString + qsTr("Permeate TDS") }

        }

        Column {
            id: units
            TableRowUnits { units: "" }
            TableRowUnits { units: "" }
            TableRowUnits { units: app.translator.emptyString + unitsText.fluxUnitText(app.units.fluxUnits) }
            TableRowUnits { units: "%" }
            TableRowUnits { units: "%" }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
            TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }

        }
    }
}
