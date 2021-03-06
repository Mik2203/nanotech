// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: container
    property bool checked
    signal toggle(bool state)

    width: 14
    height: width

    border.color: mouseArea.containsMouse ? "grey" : "lightgrey"

    border.width: 3
    property color checkedColor: "#015287"
    property color disabledColor: "lightgrey"
//    radius: 2



    states: [
        State {
            name: "UNCHECKED";  when: !container.checked && container.enabled
            PropertyChanges { target: container; color: "white"}
        },
        State {
            name: "DISABLED";  when: !container.enabled
            PropertyChanges { target: container; color: disabledColor}
        },
        State {
            name: "CHECKED";  when: container.checked
            PropertyChanges { target: container; color: checkedColor}
        }/*,
        State {
            name: "HOVER";  when: mouseArea.containsMouse
            PropertyChanges { target: container; border.color: "grey" }
        }*/
    ]

    transitions: [
        Transition {
            ColorAnimation { target: container; duration: 150}
        }

    ]

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: parent.enabled
        hoverEnabled: true
        onClicked: {
            toggle(!checked)
            container.forceActiveFocus()
        }
        onPressed: container.forceActiveFocus()
    }
}
