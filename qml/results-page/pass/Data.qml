import QtQuick 1.1

import ROPass 1.0

import "../common"
import "../stage" as StageResults
import "../util"

Column {
    id: passData
    property ROPass pass
    spacing: 10

    ElementTitle { text: app.translator.emptyString + qsTr("Pass %1").arg(sys.passIndex(pass)+1) }

    Common { pass: passData.pass }
    Hydrodynamics { pass: passData.pass }
    Solubility { element: passData.pass }
    Scaling { element: passData.pass }

    Repeater {
        model: pass.stageCount
        StageResults.Data {
            stage: pass.stage(index)
        }
    }
}
