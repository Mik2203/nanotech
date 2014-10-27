import QtQuick 1.1

import ROUnits 1.0

import "../util"

Column {
    SectionTitle { text: "Common information" }
    Row {
        spacing: 10
        Column {
            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Temperature") }
                TableRowUnits { units: app.translator.emptyString + unitsText.temperatureUnitText(app.units.temperatureUnits) }
                TableRowValue { value: app.units.convertTemperatureUnits(sys.temperature, ROUnits.DEFAULT_TEMPERATURE_UNITS, app.units.temperatureUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Water type") }
                TableRowUnits { units: app.translator.emptyString + db.waterTypes.get(sys.waterTypeIndex, "name"); width: _UNITS_WIDTH + _VALUE_WIDTH }
                //            TableRowValue { value: db.waterTypes.get(sys.waterTypeIndex, "name") }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Flow Factor") }
                TableRowUnits { units: "" }
                TableRowValue { value: sys.flowFactor }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Power") }
                TableRowUnits { units: app.translator.emptyString + qsTr("kW") }
                TableRowValue { value: sys.power }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Specific energy") }
                TableRowUnits { units: app.translator.emptyString + qsTr("kWh/m³") }
                TableRowValue { value: sys.specificEnergy }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Active Area") }
                TableRowUnits { units: app.translator.emptyString + unitsText.areaUnitText(app.units.areaUnits) }
                TableRowValue { value: app.units.convertAreaUnits(sys.activeArea, ROUnits.DEFAULT_AREA_UNITS, app.units.areaUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Total elements count") }
                TableRowUnits { units: "" }
                TableRowValue { value: sys.elementsCount }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Element lifetime") }
                TableRowUnits { units: app.translator.emptyString + qsTr("year(s)", "", sys.elementLifetime) }
                TableRowValue { value: sys.elementLifetime }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Avg. Flux") }
                TableRowUnits { units: app.translator.emptyString + unitsText.fluxUnitText(app.units.fluxUnits) }
                TableRowValue { value: app.units.convertFluxUnits(sys.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
                TableRowUnits { units: "%" }
                TableRowValue { value: sys.recovery * 100.0 }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
                TableRowUnits { units: "%" }
                TableRowValue { value: (1.0 - sys.permeate.solutes.totalValueMgl / sys.feed.solutes.totalValueMgl) * 100.0 }
            }
        }

        Column {

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Raw water flow") }
                TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
                TableRowValue { value: app.units.convertFlowUnits(sys.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Stage 1 feed flow") }
                TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
                TableRowValue { value: app.units.convertFlowUnits(sys.firstPass.firstStage.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Feed TDS") }
                TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
                TableRowValue { value: sys.feed.solutes.totalValueMgl }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Feed pressure") }
                TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
                TableRowValue { value: app.units.convertPressureUnits(sys.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Feed osmotic pressure") }
                TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
                TableRowValue { value: app.units.convertPressureUnits(sys.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Feed pH") }
                TableRowUnits { units: "" }
                TableRowValue { value: sys.feed.pH }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Permeate flow") }
                TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
                TableRowValue { value: app.units.convertFlowUnits(sys.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Permeate TDS") }
                TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
                TableRowValue { value: sys.permeate.solutes.totalValueMgl }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pressure") }
                TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
                TableRowValue { value: app.units.convertPressureUnits(sys.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Permeate osmotic pressure") }
                TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
                TableRowValue { value: app.units.convertPressureUnits(sys.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
            }

            Row {
                TableRowTitle { title: app.translator.emptyString + qsTr("Permeate pH") }
                TableRowUnits { units: "" }
                TableRowValue { value: sys.permeate.pH }
            }
        }
    }
}
