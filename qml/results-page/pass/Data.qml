import QtQuick 1.1

import ROPass 1.0

import "../common"
import "../stage" as StageResults
import "../util"
import "../util/getFlows.js" as GetFlows

Column {
    id: passData
    spacing: 10
    objectName: "passDetails"

    ElementTitle { text: app.translator.emptyString + qsTr("Passes details"); objectName: "passDetailsHeader" }

    Common { objectName: "passDetailsCommon" }
    Flows { objectName: "passDetailsStreams" }
    Scaling { objectName: "passDetailsScaling" }
}
