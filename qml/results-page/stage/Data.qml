import QtQuick 1.1

import ROStage 1.0

import "../common"
import "../element" as ElementResults
import "../util"


Column {
    id: stageData
    property ROStage stage
    spacing: 10

    ElementTitle { text: app.translator.emptyString + qsTr("Stage %1").arg(pass.stageIndex(stage)+1) }

    Common { stage: stageData.stage }
    Hydrodynamics { stage: stageData.stage }
    Solubility { element: stageData.stage }
    Scaling { element: stageData.stage }

    Repeater {
        model: stage.elementsPerVesselCount
        ElementResults.Data {
            element: stage.element(index)
        }
    }
}
