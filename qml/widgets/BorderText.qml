import QtQuick 1.1

Rectangle {
    property alias text: label.text
    property int verticalMargin: 3
    property int horizontalMargin: 3

    radius: 4
    border.color: "lightgrey"
    border.width: 1
    width: label.width + horizontalMargin * 2
    height: label.height + verticalMargin * 2


    Text {
        id: label
        anchors.centerIn: parent
    }
}
