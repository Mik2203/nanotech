import QtQuick 1.1
import ROStage 1.0
import ROStageController 1.0
import ROElementController 1.0
import Line 1.0

Item {
    property ROStage stage
    property ROStageController stageC: passC.stageC(stage)
    property int stageIndex: pass.stageIndex(stage)
    width: elWidth + cFlow.width
    height: elHeight + pFlow.height

    Row {
        anchors.bottom: box.top
        anchors.bottomMargin: editable ? -elHeight/10 : -elHeight/2
        anchors.horizontalCenter: box.horizontalCenter
        z: 2

        Text {
            text: stage.vesselCount
        }

        Text {
            text: "#"
        }

        Text {
            text: stage.elementsPerVesselCount
        }
    }

    Rectangle {
        id: box
        width: elWidth
        height: elHeight
        smooth: true

        Image {
            //id: name
            source: schemeContainer.editable && selectedStage == stage && selectedPass == pass ? "../images/scheme_stage_selected.png" : "../images/scheme_stage_normal.png"
            anchors.fill: parent
        }


        ROSchemeWarnings {
            id: stageWarnings
            visible: showWarnings && (stageC.hasAnyCautionWarnings || stageC.hasAnyCriticalWarnings)
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            function updateWarnings() { text = stageC.allWarningsMessages().join("\n") }

            Component.onCompleted: stageWarnings.updateWarnings()

            Connections {
                target: stageC
                onHasAnyCriticalWarningsChanged: stageWarnings.updateWarnings()
                onHasAnyCautionWarningsChanged: stageWarnings.updateWarnings()
            }
        }

        Image {
            source: copyButton.containsMouse ? "../images/copy1-hover.png" : "../images/copy1.png"
            anchors.left: parent.left
            anchors.leftMargin: 19
            anchors.top: parent.top
            anchors.topMargin: 8
            smooth: true
            width: height
            height: 14 //copyButton.containsMouse ? 20: 15
            visible: schemeContainer.editable && selectedStage == stage && selectedPass == pass && pass.stageCount < 4//parent.selected && elementsCount < elementsCountMax

            MouseArea {
                id: copyButton
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    selectedStage = pass.addStage(pass.stageIndex(selectedStage))
                }
            }
        }

        Image {
            source: removeButton.containsMouse ? "../images/delete1.png" : "../images/delete1-hover.png"
            //visible: parent.selected && elementsCount > elementsCountMin
            visible: schemeContainer.editable && selectedStage == stage && selectedPass == pass && (pass.stageCount > 1 || sys.passIndex(pass) > 0)
            anchors.right: parent.right
            anchors.rightMargin: 19
            anchors.top: parent.top
            anchors.topMargin: 10
            smooth: true
            width: height
            height: 12 //removeButton.containsMouse ? 20: 15
            MouseArea {
                id: removeButton
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    if (pass.stageCount > 1) {// delete stage
                        pass.removeStage(stageIndex)
                        selectedStage = pass.stage(Math.min(stageIndex, pass.stageCount-1))
                    } else { // delete pass
                        var pIdx = sys.passIndex(pass)
                        sys.removePass(pIdx)
                        selectedPass = sys.pass(Math.min(pIdx, sys.passCount-1))
                        selectedStage = selectedPass.lastStage
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: schemeContainer.editable && selectedStage != stage
            onClicked: {
                selectedPass = pass
                selectedStage = stage
            }
        }

        Line {
            id: cFlow
            property real arrowLength: calcLength()
            penWidth: linkThickness
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.right
            vertices: [0, 0,
                       arrowLength, 0]
            function calcLength() {
                var len = (elWidth+linkThickness) *0.5; // defaults
                if (stageIndex == pass.stageCount - 1) { // last stage
                    var offset = 0.0;  // вычисление максимального смещения исходя из данных рециклов всех ступеней
                    var hasRecycle = false;
                    for (var pi=0; pi<passIndex; ++pi) {
                        if (pass.hasRecycle(pi))
                            hasRecycle = true;
                        if (hasRecycle) {
                            var pOff = (sys.pass(pi).stageCount - pass.stageCount) * (elWidth + linkThickness) * 1.5 + elWidth;
                            if (pOff > offset)
                                offset = pOff;
                        }
                    }
                    len += offset;
                }
                return len;
            }

            Connections {
                target: pass
                onRecycleChanged: cFlow.arrowLength = cFlow.calcLength()
            }

            Repeater {
                model: passIndex

                Connections {
                    target: sys.pass(index)
                    onStageCountChanged: cFlow.arrowLength = cFlow.calcLength()
                }
            }

            color: "red"
            lineEndType: Line.LineEndArrow
        }


        Line {
            id: pFlow
            penWidth: linkThickness
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            vertices: [0, 0,
                       0, (elHeight+linkThickness)/2]
            color: "blue"
        }
    }
}

