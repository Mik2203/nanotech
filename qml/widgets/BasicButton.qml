// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

MouseArea {
    id: buttonArea

    property alias active: buttonArea.enabled
    property alias hovered: buttonArea.containsMouse
    property bool pressed: false
    property bool togglable: false
    property bool toggled: false
    property bool dragged: buttonArea.drag.active


    property string tooltip: ""



    hoverEnabled: true
    onPressed: {
        buttonArea.pressed = true; buttonArea.forceActiveFocus()
        if(buttonArea.togglable) buttonArea.toggled = !buttonArea.toggled;
    }  // needed when hover is enabled
    onEntered: if(pressed && enabled) buttonArea.pressed = true
    onExited: buttonArea.pressed = false
    onCanceled: buttonArea.pressed = false    // mouse stolen e.g. by Flickable
    onReleased: {
        //if(pressed && enabled) { // No click if release outside area
            buttonArea.pressed = false

        //}
    }

    Component {
        id: tooltipComponent

        Rectangle {
            id: tooltipContainer
            width: tooltipLabel.width + 5
            height: tooltipLabel.height + 5
            border.color: "lightgrey"
            border.width: 1

            Text {
                id: tooltipLabel
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 3
                text: tooltip
            }
        }
    }

    onHoveredChanged: {
        if (containsMouse && tooltip.length > 0) {
            mainWindow.autoPopup(tooltipComponent, buttonArea)
        } else if (mainWindow.popupComponent == tooltipComponent) {
            mainWindow.hidePopup()
        }
    }
}
