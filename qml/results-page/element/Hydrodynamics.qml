import QtQuick 1.1

import ROElement 1.0

import "../util"
import "../util/getFlows.js" as GetFlows
import "../common"

Column {
    property ROElement element

    SectionTitle { text: "Hydrodynamics" }

    FlowHydrodynamics {
        flows: GetFlows.forElement(element)
    }
}
