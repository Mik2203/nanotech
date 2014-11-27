import QtQuick 1.1

import ROPass 1.0

Row {
    CommonHeader {}
    Repeater {
        model: sys.passCount

        Row {
            property int passIndex: index
            property ROPass pass: sys.pass(passIndex)

            spacing: 5

            Row {
                Repeater {
                    model: pass.stageCount
                    CommonData { stage: pass.stage(index) }
                }
            }
        }
    }
}
