import QtQuick 1.1

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {

    SectionTitle { text: "Hydrodynamics" }

    FlowHydrodynamics {
        flows: GetFlows.forElement(sys, "hydrodynamics")
    }
}
