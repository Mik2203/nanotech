// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

// TODO STATES & TRANS - красоту навести

Rectangle {
    id: container

    property alias text: label.text
    property alias textFont: label.font
    property alias textSmooth: label.smooth
    property alias textWidth: label.paintedWidth
    property alias textHorizontalAlignment: label.horizontalAlignment
    property alias textVerticalAlignment: label.verticalAlignment
    property real textLeftMargin: 0
    property real textRightMargin: 0
    property real textTopMargin: 0
    property real textBottomMargin: 0

    property alias active: basicButton.active
    property alias hovered: basicButton.hovered
    property alias pressed: basicButton.pressed
    property alias togglable: basicButton.togglable
    property alias toggled: basicButton.toggled

    property color enabledColor: "white" //"#018CD1"
    property color disabledColor: "lightgrey"
    property color hoveredColor: "#caebff"
    property color toggledColor: "#018CD1"
    property color pressedColor: "#018CD1"

    property color enabledBorderColor: "grey"
    property color disabledBorderColor: "lightgrey"
    property color hoveredBorderColor: "#015287"
    property color toggledBorderColor: "#015287"
    property color pressedBorderColor: "white"

    property color enabledTextColor: "#383838" //"#018CD1"
    property color disabledTextColor: "white"
    property color hoveredTextColor: "#015287"
    property color toggledTextColor: "white"
    property color pressedTextColor: "white"

    property alias tooltip: basicButton.tooltip


    signal clicked

    height: label.paintedHeight + 10
    width: label.paintedWidth + 20

    border.width: 1
//    radius: 4
    //smooth: true
    focus:true

    color: {
        if (active) {
            if (pressed) return pressedColor
            if (toggled) return toggledColor
            if (hovered) return hoveredColor
            return enabledColor
        } else {
            return disabledColor
        }
    }

    border.color: {
        if (active) {
            if (pressed) return pressedBorderColor
            if (toggled) return toggledBorderColor
            if (hovered) return hoveredBorderColor
            return enabledBorderColor
        } else {
            return disabledBorderColor
        }
    }


    BasicButton {
        id: basicButton
        anchors.fill: parent
        hoverEnabled: true
        onClicked: container.clicked()
    }


    Text {
        id: label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: textLeftMargin
        anchors.rightMargin: textRightMargin
        anchors.topMargin: textTopMargin
        anchors.bottomMargin: textBottomMargin
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        clip: true

        color: {
            if (active) {
                if (pressed) return pressedTextColor
                if (toggled) return toggledTextColor
                if (hovered) return hoveredTextColor
                return enabledTextColor
            } else {
                return disabledTextColor
            }
        }
    }



}
