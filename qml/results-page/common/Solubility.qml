import QtQuick 1.1

import ROPass 1.0

import "../util"
import "../util/getFlows.js" as GetFlows

Column {
    property variant element

    SectionTitle { text: app.translator.emptyString + qsTr("Streams") }

    FlowSolubility {
        flows: GetFlows.forElement(element)
    }
}
