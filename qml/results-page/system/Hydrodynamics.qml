import QtQuick 1.1

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {

    SectionTitle { text: qsTr("Hydrodynamics") }

    FlowHydrodynamics {
        flows: GetFlows.forElement(sys, "hydrodynamics")
    }
}
