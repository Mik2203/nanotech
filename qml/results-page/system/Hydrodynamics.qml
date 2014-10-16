import QtQuick 1.1

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {

    SectionTitle { text: "Hydrodynamics" }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Recovery") }
        TableRowUnits { units: "%" }
        TableRowValue { value: sys.recovery * 100.0 }
    }

    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
    }

    FlowHydrodynamics {
        flows: GetFlows.forElement(sys, "hydrodynamics")
    }
}
