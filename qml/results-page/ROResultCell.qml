// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    default property alias children: rateData.children
    //property real hMargin: 10.0
    property real maxWidth: 1000.0
    property real minWidth: 10.0

    width: Math.max(Math.min(Math.max(rateData.width + border.width, parent.width), maxWidth), minWidth)
    height: rateData.height + border.width
    border.color: "grey"
    color: "transparent"
    Column {
        id: rateData
        anchors.centerIn: parent
    }
}
