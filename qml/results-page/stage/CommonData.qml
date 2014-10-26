import QtQuick 1.1

import ROUnits 1.0
import ROStage 1.0


import "../util"

Column {
    property ROStage stage

    TableRowValue { value: app.translator.emptyString + qsTr("Stage %1:%2").arg(passIndex+1).arg(index+1) }
    TableRowValue { value: stage.membrane.seriesAndModel }
    TableRowValue { value: stage.elementsCount }
    TableRowValue { value: app.units.convertFluxUnits(stage.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits) }
    TableRowValue { value: stage.recovery * 100.0 }
    TableRowValue { value: (1.0 - stage.permeate.solutes.totalValueMgl / stage.feed.solutes.totalValueMgl) * 100.0 }
    TableRowValue { value: app.units.convertFlowUnits(stage.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: stage.feed.solutes.totalValueMgl }
    TableRowValue { value: app.units.convertPressureUnits(stage.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    TableRowValue { value: app.units.convertFlowUnits(stage.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: stage.permeate.solutes.totalValueMgl }
}
