// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: container

    property variant value //: input.text
    property alias font: input.font
    property alias horizontalAlignment: input.horizontalAlignment
    property alias validator: input.validator
    property bool editable: true//: !input.readOnly
    property alias _inputText: input.text
    signal inputChanged(variant changedValue)
    property color editableBorderColor: "lightgrey"
    property color notEditableBorderColor: "grey"
    property color hoveredBorderColor: "grey"
    property color focusedBorderColor: "#2098d3"
    property color disabledBorderColor: "lightgrey"
    property alias hovered: mouseArea.containsMouse
    property alias active: mouseArea.enabled
    property alias focused: input.activeFocus

    width: 50
    height: 20
    color: editable ? "white" : "lightgrey"
//    radius: 2
    border.width: 1

    border.color: {
        if (active) {
            if (input.activeFocus) return focusedBorderColor
            if (hovered) return hoveredBorderColor
            if (editable) return editableBorderColor
            else return notEditableBorderColor
        } else {
            return disabledBorderColor
        }
    }

    onValueChanged: if (value != undefined) _inputText = value
    onInputChanged: { if (value != undefined && value != changedValue) _inputText = value }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onPressed: input.forceActiveFocus()
    }

    TextInput {
        id: input
        selectByMouse: true
        clip: true
//        focus: container.activeFocus
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 3
        anchors.verticalCenter: parent.verticalCenter
        readOnly: !editable

        onActiveFocusChanged: {
//            console.log("active focus changed!", activeFocus, text);
            if (activeFocus) selectAll()
        }

        onCursorVisibleChanged: {
            if (!cursorVisible) {
                if (value != text && editable) {
//                    console.log("input changed!", text);
                    inputChanged(text)
                }
            }
        }

        onAccepted: {
            if (value != text && editable) {
//                console.log("input changed!", text);
                inputChanged(text)
            }
        }

        KeyNavigation.tab: container.KeyNavigation.tab
        KeyNavigation.backtab: container.KeyNavigation.backtab
    }
    //}
}
