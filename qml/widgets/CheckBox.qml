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
//    radius: 2



    states: [
        State {
            name: "UNCHECKED";  when: !container.checked
            PropertyChanges { target: container; color: "white"}
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
            from: "UNCHECKED"; to: "CHECKED"
            ColorAnimation { target: container; duration: 150}
        },
        Transition {
            from: "CHECKED"; to: "UNCHECKED"
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
    }
}
