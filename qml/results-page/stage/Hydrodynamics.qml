import QtQuick 1.1

import ROStage 1.0

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {
    property ROStage stage

    SectionTitle { text: "Hydrodynamics" }

    FlowHydrodynamics {
        flows: GetFlows.forElement(stage)
    }
}
