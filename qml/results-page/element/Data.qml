import QtQuick 1.1

import ROPass 1.0
import ROStage 1.0

import "../common"
import "../element" as ElementResults
import "../util"


Column {
    spacing: 10
    objectName: "elementDetails"

    ElementTitle { text: app.translator.emptyString + qsTr("Elements details"); objectName: "elementDetailsHeader" }

    Common { objectName: "elementDetailsCommon" }
}
