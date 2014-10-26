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
}
