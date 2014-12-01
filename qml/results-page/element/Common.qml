import QtQuick 1.1

import ROPass 1.0
import ROStage 1.0

Column {
    id: elementsColumn
    property int _ELEMENTS_IN_ROW: 7
    spacing: 10

    function showElements () {
        var elements = [];
        var elementsCounter = 0;
        var rowsCounter = 0;
        var elementsRow;
        for (var passIndex=0; passIndex<sys.passCount; passIndex++) {
            var pass = sys.pass(passIndex);
            for (var stageIndex=0; stageIndex<pass.stageCount; stageIndex++) {
                var stage = pass.stage(stageIndex);
                for (var elementIndex=0; elementIndex<stage.elementsPerVesselCount; elementIndex++) {
                    if (!(elementsCounter % _ELEMENTS_IN_ROW)) {
                        // insert new row
                        var elementsRowComponent = Qt.createComponent("RowTemplate.qml");
                        elementsRow = elementsRowComponent.createObject(elementsColumn, {
                                                                            objectName: "elementDetailsCommonRow"+rowsCounter
                                                                        });
                        ++rowsCounter;
                    }
                    var dataComponent = Qt.createComponent("CommonData.qml");
                    var dataColumn = dataComponent.createObject(elementsRow, {
                                                   passIndex: passIndex,
                                                   stageIndex: stageIndex,
                                                   elementIndex: elementIndex,
                                                   element: stage.element(elementIndex)
                                               });
                    ++elementsCounter;
                }
            }
        }
    }

    Component.onCompleted: showElements()
}
