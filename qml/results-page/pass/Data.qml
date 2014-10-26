import QtQuick 1.1

import ROPass 1.0

import "../common"
import "../stage" as StageResults
import "../util"
import "../util/getFlows.js" as GetFlows

Column {
    id: passData
    spacing: 10

    ElementTitle { text: app.translator.emptyString + qsTr("Passes") }

    Row {
        CommonHeader {}
        Repeater {
            model: sys.passCount
            CommonData { pass: sys.pass(index) }
        }
    }

    Row {
        FlowsHeader {}

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
                    FlowsData { flows: GetFlows.forElement(sys.pass(index)) }
                }
            }
        }
    }
}
