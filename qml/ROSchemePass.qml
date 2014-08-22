import QtQuick 1.0
import ROPass 1.0
import ROPassController 1.0
import Line 1.0

import "widgets" as ROWidgets

Item {
    id: container
    property int passPermeateArrowLength: 70
    property int passFeedArrowLength: 90
    property int passBlendArrowRightOffset: 30
    property ROPass pass
    property ROPassController passC: sysC.passC(pass)
    property int passIndex: sys.passIndex(pass)
    width: stages.width
    height: stages.height + pFlow.height
    property bool passHasIncomingRecycles: __updatePassHasIncomingRecycles()

    function __updatePassHasIncomingRecycles() {
        for (var pi=passIndex+1; pi<sys.passCount; ++pi) {
            if (sys.pass(pi).hasRecycle(passIndex))
                return true;
        }
        return false;
    }

    Repeater {
        model: sys.passCount

        Connections {
            target: sys.pass(index)
            onRecycleChanged: passHasIncomingRecycles = __updatePassHasIncomingRecycles()
        }
    }

    Row {
        id: stages

        Line {
            penWidth: linkThickness
            anchors.top: parent.top
            anchors.topMargin: (elHeight-linkThickness) / 2
            vertices: [0, 0,
                       passFeedArrowLength, 0]
            color: passIndex == 0 ? "black" : "blue"
            lineEndType: Line.LineEndArrow

            ROWidgets.BorderText {  // feed to stages label
                id: feedStageLabel
                opacity: 0.85
                visible: passHasIncomingRecycles || pass.hasSelfRecycle || pass.hasBlendPermeate
                anchors.right: parent.right
                anchors.rightMargin: 15
                anchors.bottom: parent.top
                anchors.bottomMargin: -(height - linkThickness) / 2
                text: app.translator.emptyString + qsTr("SF%1").arg(passIndex+1)
            }

            ROWidgets.BorderText {  // feed label
                id: feedLabel
                opacity: 0.85
                visible: passHasIncomingRecycles
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: -(height + linkThickness) / 2
                text: app.translator.emptyString + qsTr("F%1").arg(passIndex+1)
            }
        }

        Repeater {
            model: pass.stageCount
            delegate: ROSchemeStage { stage: pass.stage(index)  }
        }
    }

    Line {
        id: pFlow
        penWidth: linkThickness
        anchors.left: parent.left
        anchors.top: stages.bottom
        vertices: [getWidth(), 0,
                   0, 0,
                   0, passPermeateArrowLength - linkThickness * 2]
        color: "blue"
        function getWidth(){
            return passFeedArrowLength + elWidth / 2 + linkThickness * 0.5 + (pass.stageCount-1) * (elWidth * 1.5 + linkThickness * 1.3);
        }

        lineEndType: passIndex == sys.passCount-1 ? Line.LineEndArrow : Line.LineEndNone

        ROWidgets.BorderText {  // total permeate label
            anchors.left: parent.left
            opacity: 0.85
            anchors.leftMargin: -width / 2
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("P%1").arg(passIndex+1)
        }
    }

    ROWidgets.Button {
        text: app.translator.emptyString + qsTr("+")
        anchors.left: parent.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.top
        anchors.verticalCenterOffset: (elHeight - linkThickness) * 0.5
        width: 18
        height: width
        onClicked: { pass.addStage(pass.stageCount); selectedPass = pass; selectedStage = pass.lastStage;  }
        visible: schemeContainer.editable && pass.stageCount < pass.MAX_STAGES_COUNT
        tooltip: qsTr("Add stage")
    }

    Line { // SELF RECYCLE
        anchors.right: parent.right
        anchors.rightMargin: arrowLength / 2
        anchors.bottom: parent.top // привязка к высшей точке - это точка прямоугольников стадий
        anchors.bottomMargin: -elHeight / 2 //Сдвиг вниз

        visible: pass.hasSelfRecycle

        color: "red"
        penWidth: linkThickness
        vertices: [0, 0,
                   0, -elHeight,
                   parent.width - passFeedArrowLength + 30, -elHeight,
                   parent.width - passFeedArrowLength + 30, 0]
        lineStartType: Line.LineEndArrow

        ROWidgets.BorderText {
            opacity: 0.85
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: -height / 2
            text: app.translator.emptyString + qsTr("SR%1").arg(passIndex+1)
        }

        ROWidgets.BorderText {  // concentrate from stages label
            opacity: 0.85
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -(height - linkThickness * 2) / 2
            text: app.translator.emptyString + qsTr("SC%1").arg(passIndex+1)
        }
    }

    Line { // BLEND
        anchors.left: parent.left  // привязка к левой стороне первой стадии
        anchors.leftMargin: passBlendArrowRightOffset  // сдвиг вправо
        anchors.top: parent.top  // привязка к высшей точке - это точка прямоугольников стадий
        anchors.topMargin: (elHeight) * 0.5  //сдвиг вниз

        visible: pass.hasBlendPermeate

        color: passIndex == 0 ? "black" : "blue"
        penWidth: linkThickness
        vertices: [0, 0,
                   0, elHeight]
        lineEndType: Line.LineEndArrow

        ROWidgets.BorderText {
            opacity: 0.85
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: elHeight * 0.2
            text: app.translator.emptyString + qsTr("B%1").arg(passIndex+1)
        }

        ROWidgets.BorderText {  // permeate from stages label
            opacity: 0.85
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.top: parent.bottom
            anchors.topMargin: -height/2
            text: app.translator.emptyString + qsTr("SP%1").arg(passIndex+1)
        }
    }

    Repeater {  // рециклы на другие ступени
        model: passIndex

        Line {
            id: rFlow
            visible: pass.hasRecycle(index)
            penWidth: linkThickness
            anchors.right: parent.right
            anchors.rightMargin: arrowLength / 2
            anchors.bottom: parent.top // привязка к высшей точке - это точка прямоугольников стадий
            anchors.bottomMargin: -elHeight / 2 //Сдвиг вниз
            vertices: [0, 0,
                0, -(passPermeateArrowLength + elHeight) * (passIndex - index) - elHeight * 1.5, //-linkThickness*2
                -stages.width + arrowLength / 2 + 42, -(passPermeateArrowLength + elHeight) * (passIndex - index) - elHeight * 1.5, //-(passOffset-linkThickness*2 + stages.height ) * (passIndex - index) - 10
                -stages.width + arrowLength / 2 + 42, -(passPermeateArrowLength + elHeight) * (passIndex - index)] //-(passOffset-linkThickness*2 + stages.height ) * (passIndex - index) + elHeight - 16
            color: "red"
            lineEndType: Line.LineEndArrow
            Connections {
                target: pass
                onRecycleChanged: rFlow.visible = pass.hasRecycle(index)
            }

            ROWidgets.BorderText {
                opacity: 0.85
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: -height / 2
                text: app.translator.emptyString + qsTr("R%1-%2").arg(passIndex+1).arg(index+1)
            }
        }


    }
}
