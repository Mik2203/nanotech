import QtQuick 1.1
import ROPass 1.0
import Line 1.0

import "widgets" as ROWidgets

Item {
    id: schemeContainer
    width: passRow.width
    height: passRow.height + elHeight

    property real elWidth: 70
    property real elHeight: 40
    property real arrowLength: elWidth/2
    property real linkThickness: 3
    property real passOffset: 50
    property bool editable: true
    property bool showWarnings: false



    Row {
        id: passRow

        anchors.top: parent.top
        anchors.topMargin: elHeight

        Line {
            penWidth: linkThickness
            anchors.top: parent.top
            anchors.topMargin: (elHeight-linkThickness) / 2
            vertices: [0, 0,
                       elWidth/2, 0]
            color: "black"
            lineEndType: Line.LineEndArrow
            //lineStartType: Line.LineEndArrow
        }

        Item {
            id: passes

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
                    x: index * elWidth
                    y: index * (elHeight + passOffset)

                    onWidthChanged: passes.updateWidth()

                    Line {
                        visible: sys.passCount-1 > index

                        penWidth: linkThickness
                        anchors.top: parent.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: (elWidth - linkThickness) / 2

                        vertices: [0, 0,
                                   0, passOffset-linkThickness*2,
                                   elWidth/2, passOffset-linkThickness*2]
                        color: "blue"
                        lineEndType: Line.LineEndArrow
                    }


                    ROWidgets.Button {
                        text: app.translator.emptyString + qsTr("+")
                        anchors.top: parent.bottom
                        anchors.topMargin: 5
                        anchors.left: parent.left
                        anchors.leftMargin: 25
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
