import QtQuick 1.1

import ROPass 1.0
import ROStage 1.0

import "../common"
import "../element" as ElementResults
import "../util"


Column {
    id: stageData
    spacing: 10
    objectName: "stageDetails"

    ElementTitle { text: app.translator.emptyString + qsTr("Stages details"); objectName: "stageDetailsHeader" }

    Common { objectName: "stageDetailsCommon" }
    Flows { objectName: "stageDetailsStreams" }
}
