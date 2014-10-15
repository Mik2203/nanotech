import QtQuick 1.1

import "../util"
import "../common"

Column {
    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
    }

    FlowScaling {
        flows: [
            { title: qsTr("Feed"), flow: sys.feed },
            { title: qsTr("Adj. feed"), flow: sys.adjustedFeed },
            { title: qsTr("Stage 1 feed"), flow: sys.firstPass.firstStage.feed },
            { title: qsTr("Permeate"), flow: sys.permeate },
            { title: qsTr("Concentrate"), flow: sys.concentrate }
        ]
    }
}
