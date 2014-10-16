import QtQuick 1.1

import ROUnits 1.0
import ROStage 1.0


import "../util"

Column {
    property ROStage stage

    SectionTitle { text: "Common" }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Membrane element") }
        TableRowUnits { units: "" }
        TableRowValue { value: stage.membrane.seriesAndModel }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Active Area") }
        TableRowUnits { units: app.translator.emptyString + unitsText.areaUnitText(app.units.areaUnits) }
        TableRowValue { value: app.units.convertAreaUnits(stage.activeArea, ROUnits.DEFAULT_AREA_UNITS, app.units.areaUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Total elements count") }
        TableRowUnits { units: "" }
        TableRowValue { value: stage.elementsCount }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Avg. Flux") }
        TableRowUnits { units: app.translator.emptyString + unitsText.fluxUnitText(app.units.fluxUnits) }
        TableRowValue { value: app.units.convertFluxUnits(stage.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
        TableRowUnits { units: "%" }
        TableRowValue { value: stage.recovery * 100.0 }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
        TableRowUnits { units: "%" }
        TableRowValue { value: (1.0 - stage.permeate.solutes.totalValueMgl / stage.feed.solutes.totalValueMgl) * 100.0 }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed flow") }
        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        TableRowValue { value: app.units.convertFlowUnits(stage.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed TDS") }
        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
        TableRowValue { value: stage.feed.solutes.totalValueMgl }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(stage.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed osmotic pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(stage.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Feed pH") }
        TableRowUnits { units: "" }
        TableRowValue { value: stage.feed.pH }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate flow") }
        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        TableRowValue { value: app.units.convertFlowUnits(stage.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate TDS") }
        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
        TableRowValue { value: stage.permeate.solutes.totalValueMgl }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(stage.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate osmotic pressure") }
        TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        TableRowValue { value: app.units.convertPressureUnits(stage.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pH") }
        TableRowUnits { units: "" }
        TableRowValue { value: stage.permeate.pH }
    }
}
