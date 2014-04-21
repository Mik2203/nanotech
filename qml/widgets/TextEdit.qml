// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: container

    property string value //: input.text
    property alias horizontalAlignment: input.horizontalAlignment
    property bool editable: true//: !input.readOnly
    signal inputChanged(string changedValue)

    width: 50
    height: 20
    color: editable ? "white" : "lightgrey"
    radius: 2
    border.color: getBorderColor()
    border.width: 1


    function getBorderColor(){
        if (input.activeFocus)
            return editable ? "#2098d3" : "grey"
        if (mouseArea.containsMouse)
            return "gray"
        return "lightgray"
    }

    onValueChanged: input.text = value;

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
    }

    ScrollArea {
        anchors.fill: parent
        frame: false

        Item {
            id: inputContainer
            x: 3
            y: 3
            width: Math.max(input.paintedWidth, container.width) // DONT KNOW WHY -10 ???
            height: Math.max(input.paintedHeight, container.height)

            TextEdit {
                id: input
                selectByMouse: true
                clip: true
                focus: container.activeFocus
                width: container.width
                height: container.height
                readOnly: !editable

                onActiveFocusChanged: {
                    if (activeFocus) selectAll()
                }

                onTextChanged: inputChanged(text)

                onCursorVisibleChanged: {
                    if (!cursorVisible) {
                        if (container.value != text && editable) {
                            inputChanged(text)
                        }
                    }
                }

                KeyNavigation.tab: container.KeyNavigation.tab
                KeyNavigation.backtab: container.KeyNavigation.backtab
            }
        }


    }
    //}
}
