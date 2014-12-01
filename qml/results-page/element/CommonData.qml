import QtQuick 1.1

import ROUnits 1.0
import ROElement 1.0


import "../util"

Column {
    property int passIndex
    property int stageIndex
    property int elementIndex
    property ROElement element

    TableRowValue { value: app.translator.emptyString + qsTr("Element %1:%2:%3").arg(passIndex+1).arg(stageIndex+1).arg(elementIndex+1) }
    TableRowValue { value: app.units.convertFluxUnits(element.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits) }
    TableRowValue { value: element.recovery * 100.0 }
    TableRowValue { value: (1.0 - element.permeate.solutes.totalValueMgl / element.feed.solutes.totalValueMgl) * 100.0 }
    TableRowValue { value: app.units.convertFlowUnits(element.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: element.feed.solutes.totalValueMgl }
    TableRowValue { value: app.units.convertPressureUnits(element.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
    TableRowValue { value: app.units.convertFlowUnits(element.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    TableRowValue { value: element.permeate.solutes.totalValueMgl }
}
