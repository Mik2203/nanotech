import QtQuick 1.1

import ROUnits 1.0
import ROElement 1.0


import "../util"

Column {
    property ROElement element

    SectionTitle { text: "Common" }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Membrane element") }
        TableRowUnits { units: "" }
        TableRowValue { value: stage.membrane.seriesAndModel }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Active Area") }
        TableRowUnits { units: app.translator.emptyString + unitsText.areaUnitText(app.units.areaUnits) }
        TableRowValue { value: app.units.convertAreaUnits(element.activeArea, ROUnits.DEFAULT_AREA_UNITS, app.units.areaUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Avg. Flux") }
        TableRowUnits { units: app.translator.emptyString + unitsText.fluxUnitText(app.units.fluxUnits) }
        TableRowValue { value: app.units.convertFluxUnits(element.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
        TableRowUnits { units: "%" }
        TableRowValue { value: element.recovery * 100.0 }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
        TableRowUnits { units: "%" }
        TableRowValue { value: (1.0 - element.permeate.solutes.totalValueMgl / element.feed.solutes.totalValueMgl) * 100.0 }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed flow") }
        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        TableRowValue { value: app.units.convertFlowUnits(element.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed TDS") }
        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
        TableRowValue { value: element.feed.solutes.totalValueMgl }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(element.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed osmotic pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(element.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed pH") }
        TableRowUnits { units: "" }
        TableRowValue { value: element.feed.pH }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate flow") }
        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        TableRowValue { value: app.units.convertFlowUnits(element.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate TDS") }
        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
        TableRowValue { value: element.permeate.solutes.totalValueMgl }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(element.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate osmotic pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(element.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pH") }
        TableRowUnits { units: "" }
        TableRowValue { value: element.permeate.pH }
    }
}
