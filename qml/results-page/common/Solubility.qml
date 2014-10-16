import QtQuick 1.1

import ROPass 1.0

import "../util"
import "../util/getFlows.js" as GetFlows

Column {
    property variant element

    SectionTitle { text: "Solubility" }

    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
        TableRowUnits { units: "%" }
        TableRowValue { value: (1.0 - element.permeate.solutes.totalValueMgl / element.feed.solutes.totalValueMgl) * 100.0 }
    }

    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
    }

    FlowSolubility {
        flows: GetFlows.forElement(element)
    }
}
