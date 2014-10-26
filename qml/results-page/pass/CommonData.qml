import QtQuick 1.1

import ROUnits 1.0
import ROPass 1.0


import "../util"

Column {
    property ROPass pass
//    property int passIndex: sys.passIndex(pass)

    Item { height: _ROW_HEIGHT }

    TableRowValue { value: app.translator.emptyString + qsTr("Pass %1").arg(sys.passIndex(pass)+1) }
    TableRowValue { value: pass.power }
    TableRowValue { value: pass.specificEnergy }
    TableRowValue { value: app.units.convertAreaUnits(pass.activeArea, ROUnits.DEFAULT_AREA_UNITS, app.units.areaUnits) }
    TableRowValue { value: pass.elementsCount }
    TableRowValue { value: app.units.convertFluxUnits(pass.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits) }
    TableRowValue { value: pass.recovery * 100.0 }
    TableRowValue { value: pass.permeate.rate/(pass.feed.rate+pass.selfRecycle) * 100.0 }
    TableRowValue { value: (1.0 - pass.permeate.solutes.totalValueMgl / pass.feed.solutes.totalValueMgl) * 100.0 }
    TableRowValue { value: app.units.convertFlowUnits(pass.rawWater.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: app.units.convertFlowUnits(pass.blendPermeate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: app.units.convertFlowUnits(pass.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: app.units.convertFlowUnits(pass.selfRecycle, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }

    Repeater {
        model: sys.passCount-1

        TableRowValue { value: app.units.convertFlowUnits(pass.recycle(index), ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    TableRowValue { value: app.units.convertFlowUnits(pass.firstStage.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: app.units.convertFlowUnits(pass.totalProduct.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }


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
