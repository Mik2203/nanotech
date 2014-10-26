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

    FlowHydrodynamics {
        flows: GetFlows.forElement(pass)
    }
}
