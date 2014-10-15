import QtQuick 1.1

import "../util"
import "../common"

Column {
    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
        TableRowUnits { units: "%" }
        TableRowValue { value: (1.0 - sys.permeate.solutes.totalValueMgl / sys.feed.solutes.totalValueMgl) * 100.0 }
    }

    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
    }

    FlowSolubility {
        flows: [
            { title: qsTr("Feed"), flow: sys.feed },
            { title: qsTr("Adj. feed"), flow: sys.adjustedFeed },
            { title: qsTr("Stage 1 feed"), flow: sys.firstPass.firstStage.feed },
            { title: qsTr("Permeate"), flow: sys.permeate },
            { title: qsTr("Concentrate"), flow: sys.concentrate }
        ]
    }
}
