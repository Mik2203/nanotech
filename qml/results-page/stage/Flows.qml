import QtQuick 1.1

import ROPass 1.0

import "../util"
import "../common"

Row {
    FlowsHeader {}

    Repeater {
        model: sys.passCount

        Row {
            property int passIndex: index
            property ROPass pass: sys.pass(passIndex)

            spacing: 5
            Row {
                Repeater {
                    model: pass.stageCount
                    Column {
                        TableRowValue { value: app.translator.emptyString + qsTr("Stage %1:%2").arg(passIndex+1).arg(index+1) }
                        FlowsData { flows: [{ title: app.translator.emptyString + qsTr("Permeate"), flow: pass.stage(index).permeate }] }
                    }
                }
            }
        }
    }
}
