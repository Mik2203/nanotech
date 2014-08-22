import QtQuick 1.1

Item {
    id: slider
    property real min: 0
    property real max: 1
    property real value: min + (max - min) * (bar.x / (foo.width - bar.width))
    property real init: min+(max-min)/2
    property alias tooltip: sliderController.tooltip

    Component.onCompleted: setValue(init)
    function setValue(v) {
       if (min < max)
          bar.x = v/(max - min) * (foo.width - bar.width);
    }

    Item {
        id: foo
        height: 6
        anchors.verticalCenter:parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.right: parent.right
        anchors.rightMargin: 6
        Rectangle {
            height: parent.height
            anchors.left: parent.left
            anchors.right: bar.horizontalCenter
            color: "#caebff"
//            radius: 3
        }
        Rectangle {
            height: parent.height
            anchors.left: bar.horizontalCenter
            anchors.right: parent.right
            color: "lightgrey"
//            radius: 3
        }
        Rectangle {
            anchors.fill: parent
            color: "transparent"
//            radius: 3
            border.width: 1
            border.color: "grey"
        }

        Rectangle {
            id: bar
            y: -5
            smooth: true
            width: 15
            height: 15
//            radius: 15

            property color enabledColor: "white"
            property color disabledColor: "lightgrey"
            property color hoveredColor: "#caebff"
            property color pressedColor: "#018CD1"

            property color enabledBorderColor: "grey"
            property color disabledBorderColor: "lightgrey"
            property color hoveredBorderColor: "#015287"
            property color pressedBorderColor: "white"

            color: {
                if (enabled) {
                    if (sliderController.pressed || sliderController.dragged) return pressedColor
                    if (sliderController.hovered) return hoveredColor
                    return enabledColor
                } else {
                    return disabledColor
                }
            }

            border.width: 1
            border.color:  {
                if (enabled) {
                    if (sliderController.pressed || sliderController.dragged) return pressedBorderColor
                    if (sliderController.hovered) return hoveredBorderColor
                    return enabledBorderColor
                } else {
                    return disabledBorderColor
                }
            }

            BasicButton {
                id: sliderController
                anchors.fill: parent
                hoverEnabled: true
                drag.target: parent
                drag.axis: Drag.XAxis
                drag.minimumX: 0
                drag.maximumX: foo.width - width
            }
        }
    }
}
