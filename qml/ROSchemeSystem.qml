import QtQuick 1.1
import ROPass 1.0
import Line 1.0
import ROScalingElement 1.0

import "widgets" as ROWidgets

Item {
    id: schemeContainer
    width: passRow.width + 15  // 15 - labels, etc..
    height: passRow.height + elHeight * 2  // 1 elHeight запасная на рециклы

    property real elWidth: 70
    property real elHeight: 40
    property real arrowLength: elWidth/2
    property real linkThickness: 3
    property bool editable: true
    property bool showWarnings: false



    Row {
        id: passRow

        anchors.top: parent.top
        anchors.topMargin: elHeight * 1.5  // 1 elHeight запасная на рециклы

        anchors.left: parent.left
        anchors.leftMargin: 15  // 15 - labels, etc..

        Column {
            id: adjustmentIndicator
            anchors.bottom: parent.top
            anchors.bottomMargin: -(elHeight + linkThickness) / 2
            visible: sys.scalingElement.adjustment != ROScalingElement.NoAdjusment



            Image {
                source: "../images/acid.png";
                width: 30
                height: 30
                anchors.right: parent.right
                smooth: true
                rotation: 120
            }

            Line {
                penWidth: linkThickness
                anchors.right: parent.right
                vertices: [0, 0,
                           0, elWidth/4]
                color: "green"
                lineEndType: Line.LineEndArrow
            }

            Line {
                penWidth: linkThickness
                vertices: [0, 0,
                           elWidth/4, 0]
                anchors.right: parent.right
                color: "black"
            }
        }

        Column {
            id: passes
            spacing: -elHeight / 2

            function updateHeight() {
                var spi = passesRepeater.itemAt(passesRepeater.count-1)
                height = spi.y + spi.height;
            }
            function updateWidth() {
                var maxX = 0.0;
                for (var pi=0; pi<sys.passCount; ++pi) {
                    var spi = passesRepeater.itemAt(pi)
                    maxX = Math.max(maxX, spi.x + spi.width)
                }
                width = maxX
            }

            Repeater {
                id: passesRepeater
                model: sys.passCount
                onItemAdded: { passes.updateWidth(); passes.updateHeight(); }
                onItemRemoved: { passes.updateWidth(); passes.updateHeight(); }

                delegate: ROSchemePass {
                    pass: sys.pass(index)
                    passIndex: index

                    onWidthChanged: passes.updateWidth()

                    ROWidgets.Button {
                        text: app.translator.emptyString + qsTr("+")
                        anchors.top: parent.bottom
                        anchors.topMargin: 5
                        anchors.left: parent.left
                        anchors.leftMargin: elWidth - width / 2
                        width: 18
                        height: width
                        onClicked: { sys.addPass(index+1); selectedPass = sys.pass(index+1) }
                        visible: schemeContainer.editable && index == sys.passCount-1 && sys.passCount < sys.MAX_PASSES_COUNT
                    }
                }
            }
        }
    }
}
