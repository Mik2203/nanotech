import QtQuick 1.0
import ROPass 1.0
import ROPassController 1.0
import Line 1.0

import "widgets" as ROWidgets

Item {
    id: container
    property ROPass pass
    property ROPassController passC: sysC.passC(pass)
    property int passIndex: sys.passIndex(pass)
    width: stages.width
    height: stages.height

    Row {
        id: stages
        Repeater {
            model: pass.stageCount
            delegate: ROSchemeStage { stage: pass.stage(index)  }
        }
    }

    Line {
        id: pFlow
        penWidth: linkThickness
        anchors.left: parent.left
        anchors.leftMargin: (elWidth-linkThickness) / 2
        anchors.bottom: stages.bottom
        vertices: [0, 0,
                   getWidth(), 0]
        color: "blue"
        function getWidth(){
            if (passIndex < sys.passCount-1) {
                if (pass.stageCount == 1) {
                    if (sys.passCount > 1) {
                        return 0;
                    } else {
                        return elWidth
                    }
                }
                return (elWidth*1.5 + linkThickness) * (pass.stageCount-1)
            } else { // LAST PASS
                return stages.width - elWidth/2
            }
        }

        lineEndType: passIndex == sys.passCount-1 ? Line.LineEndArrow : Line.LineEndNone
    }

    ROWidgets.Button {
        text: app.translator.emptyString + qsTr("+")
        anchors.left: parent.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.top
        anchors.verticalCenterOffset: elHeight/2 - 2
        width: 18
        height: width
        onClicked: { pass.addStage(pass.stageCount); selectedPass = pass; selectedStage = pass.lastStage;  }
        visible: schemeContainer.editable && pass.stageCount < pass.MAX_STAGES_COUNT
    }

    Line { // SELF RECYCLE
        anchors.left: parent.left // привязка по сути к левой стороне первой стадии
        anchors.leftMargin: -(arrowLength + linkThickness)/2 // Сдвиг влево на половину длины соединяющей стрелки
        anchors.bottom: parent.top // привязка к высшей точке - это точка прямоугольников стадий
        anchors.bottomMargin: -elHeight/2 //Сдвиг вниз

        visible: pass.hasSelfRecycle

        color: "red"
        penWidth: linkThickness
        vertices: [0, 0,
                   0, -elHeight * 0.7,
                   parent.width, -elHeight * 0.7,
                   parent.width, 0]
        lineStartType: Line.LineEndArrow
    }

    Line { // BLEND
        anchors.left: parent.left // привязка по сути к левой стороне первой стадии
        anchors.leftMargin: -arrowLength/2 - linkThickness*3 // Сдвиг влево на половину длины соединяющей стрелки
        anchors.top: parent.top // привязка к высшей точке - это точка прямоугольников стадий
        anchors.topMargin: elHeight / 2 //Сдвиг вниз

        visible: pass.hasBlendPermeate

        color: passIndex == 0 ? "black" : "blue"
        penWidth: linkThickness
        vertices: [0, 0,
                   0, elHeight,
                   (elWidth + arrowLength) / 2 + linkThickness*2, elHeight]
        lineEndType: Line.LineEndArrow
    }

    Repeater {
        model: passIndex

        Line {
            id: rFlow
            visible: pass.hasRecycle(index)
            penWidth: linkThickness
            anchors.right: parent.right
            anchors.rightMargin: (arrowLength - linkThickness) / 2
            anchors.bottom: parent.top // привязка к высшей точке - это точка прямоугольников стадий
            anchors.bottomMargin: -elHeight / 2 //Сдвиг вниз
            vertices: [0, 0,
                0, -(passOffset + elHeight) * (passIndex - index) - elHeight*0.9, //-linkThickness*2
                -stages.width - elWidth * (passIndex - index), -(passOffset + elHeight) * (passIndex - index) - elHeight*0.9, //-(passOffset-linkThickness*2 + stages.height ) * (passIndex - index) - 10
                -stages.width - elWidth * (passIndex - index), -(passOffset + elHeight) * (passIndex - index)] //-(passOffset-linkThickness*2 + stages.height ) * (passIndex - index) + elHeight - 16
            color: "red"
            lineEndType: Line.LineEndArrow
            Connections {
                target: pass
                onRecycleChanged: rFlow.visible = pass.hasRecycle(index)
            }
        }


    }
}
