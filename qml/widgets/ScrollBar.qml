import QtQuick 1.1
import RangeModel 0.1

Item {
    id: scrollbar

    property int orientation: Qt.Horizontal
    property alias minimumValue: slider.minimumValue
    property alias maximumValue: slider.maximumValue
    property alias value: slider.value
    property bool scrollToClickposition: true
    property int barCompactWidth: 4
    property int barExpandWidth: 10

    implicitWidth: orientation == Qt.Horizontal ? 200 : barExpandWidth
    implicitHeight: orientation == Qt.Horizontal ? barExpandWidth : 200

    Rectangle {
        color: "lightgrey"
        border.color: "grey"
        border.width: 1
//        radius: 2
        smooth: true
        width: orientation == Qt.Horizontal ? scrollbar.width
                                            : internal.containsMouse || internal.pressed ? barExpandWidth : barCompactWidth
        height: orientation == Qt.Vertical ? scrollbar.height
                                           : internal.containsMouse || internal.pressed ? barExpandWidth : barCompactWidth
        anchors.right: if (orientation == Qt.Vertical) parent.right
        anchors.bottom: if (orientation == Qt.Horizontal) parent.bottom
        Behavior on height { NumberAnimation { duration: 100 } }
        Behavior on width { NumberAnimation { duration: 100 } }
    }


    MouseArea {
        id: internal
        property real pressedX
        property real pressedY
        property int oldPosition
        property bool holdSlider: false
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        onPositionChanged: {
            if (pressed && holdSlider) {
                if (scrollbar.orientation != Qt.Horizontal) {
                    slider.position = oldPosition + (mouseY - pressedY)
                }
                else {
                    slider.position = oldPosition + (mouseX - pressedX)
                }
            }
        }

        onPressed: {
            pressedX = mouseX
            pressedY = mouseY
            oldPosition = slider.position
            if ((orientation == Qt.Vertical && mouseY > sliderBar.y && mouseY < sliderBar.y + sliderBar.height) ||
                    (orientation == Qt.Horizontal && mouseX > sliderBar.x && mouseX < sliderBar.x + sliderBar.width))
                holdSlider = true
            else {
                slider.position = orientation == Qt.Horizontal ? mouseX - sliderBar.width/2
                                                               : mouseY - sliderBar.height/2
                holdSlider = false
            }
        }


        Rectangle {
            id: sliderBar
            color: "grey"
//            radius: 2
            smooth: true
            width: orientation == Qt.Horizontal ? scrollbar.width*scrollbar.width / (scrollbar.width + scrollbar.maximumValue)
                                                : internal.containsMouse || internal.pressed ? barExpandWidth : barCompactWidth
            height: orientation == Qt.Vertical ? scrollbar.height*scrollbar.height / (scrollbar.height + scrollbar.maximumValue)
                                               : internal.containsMouse || internal.pressed ? barExpandWidth : barCompactWidth
            anchors.right: if (orientation == Qt.Vertical) parent.right
            anchors.bottom: if (orientation == Qt.Horizontal) parent.bottom
            x: orientation == Qt.Horizontal ? slider.position : 0.0
            y: orientation == Qt.Vertical ? slider.position : 0.0
            Behavior on height { NumberAnimation { duration: 100 } }
            Behavior on width { NumberAnimation { duration: 100 } }
            //Behavior on x { NumberAnimation { duration: 100 } }
            //Behavior on y { NumberAnimation { duration: 100 } }
        }

        RangeModel {
            id: slider
            minimumValue: 0.0
            maximumValue: 1.0
            value: 0
            stepSize: 0.0
            inverted: false
            //Behavior on position { NumberAnimation { duration: 100 } }
            positionAtMaximum: orientation == Qt.Horizontal ? scrollbar.width - sliderBar.width
                                                            : scrollbar.height - sliderBar.height //internal.grooveSize
        }
    }

}
