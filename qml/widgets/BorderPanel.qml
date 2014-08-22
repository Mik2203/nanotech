import QtQuick 1.1

Rectangle { // BORDER
    border.color: "grey"
    border.width: 1
    color: "transparent"
    property string headerLabel
//    radius: 3

    Rectangle {
        anchors.verticalCenter: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: headerLabelText.width + 10
        height: headerLabelText.height + 5

        Text {
            id: headerLabelText
            text: headerLabel
            anchors.centerIn: parent
        }
    }
}
