import QtQuick 1.1

import ROPass 1.0
import ROStage 1.0

import "../common"
import "../element" as ElementResults
import "../util"


Column {
    spacing: 10

    ElementTitle { text: app.translator.emptyString + qsTr("Elements details") }

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

                        Row {
                            property int stageIndex: index
                            property ROStage stage: pass.stage(stageIndex)
                            Repeater {
                                model: stage.elementsPerVesselCount
                                CommonData { element: stage.element(index) }
                            }
                        }
                    }
                }
            }
        }
    }
}
