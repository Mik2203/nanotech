import QtQuick 1.1

import ROStage 1.0

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {
    property ROStage stage

    SectionTitle { text: "Hydrodynamics" }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
        TableRowUnits { units: "%" }
        TableRowValue { value: stage.recovery * 100.0 }
    }

    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
    }

    FlowHydrodynamics {
        flows: GetFlows.forElement(stage)
    }
}
