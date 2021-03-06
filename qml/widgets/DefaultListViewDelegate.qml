// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: delegateRectangle
    width: 100
    height: 18
    color: selected ? "#018CD1" : (hovered ? "#caebff" : "white")
    border.width: 1
    border.color: "grey"

    property bool hovered: index == listView.currentIndex
    property bool selected: false
//    radius: 2

    property alias text: label.text
    property alias textFont: label.font
    property alias textColor: label.color

    MouseArea {
        id: delegateSelector
        anchors.fill: parent
        hoverEnabled: true

        onEntered: listView.currentIndex = index
        onClicked: { select(index); close(); }
    }

    Text {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: 3
    }
}

