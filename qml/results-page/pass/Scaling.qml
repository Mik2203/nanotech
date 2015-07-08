import QtQuick 1.1

import "../util/getFlows.js" as GetFlows
import "../common"

Row {
    ScalingHeader {}

    Row {
        spacing: 5
        Repeater {
            model: sys.passCount
            Column {
                Text {
                    text: app.translator.emptyString + qsTr("Pass %1").arg(index+1)
                    anchors.horizontalCenter: parent.horizontalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.underline: true
                    height: _ROW_HEIGHT
                }
                ScalingData { flows: GetFlows.forElement(sys.pass(index)) }
            }
        }
    }
}
