import QtQuick 1.1

import ROPass 1.0
import ROUnits 1.0

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {
    property ROPass pass
    property int passIndex: sys.passIndex(pass)

    SectionTitle { text: "Hydrodynamics" }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
        TableRowUnits { units: "%" }
        TableRowValue { value: pass.recovery * 100.0 }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Average stages recovery") }
        TableRowUnits { units: "%" }
        TableRowValue { value: pass.permeate.rate/(pass.feed.rate+pass.selfRecycle) * 100.0 }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Self recycle") }
        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        TableRowValue { value: app.units.convertFlowUnits(pass.selfRecycle, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Blending") }
        TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        TableRowValue { value: app.units.convertFlowUnits(pass.blendPermeate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
    }

    Repeater {
        model: passIndex

        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Recycle to pass %1").arg(index+1) }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
            TableRowValue { value: app.units.convertFlowUnits(pass.recycle(index), ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
        }
    }

    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
    }

    FlowHydrodynamics {
        flows: GetFlows.forElement(pass)
    }
}
