import QtQuick 1.1

import "../util"
import "../util/getFlows.js" as GetFlows


Column {
    property variant element

    SectionTitle { text: qsTr("Scaling") }

    FlowScaling {
        flows: GetFlows.forElement(element)
    }
}
