// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    property real value
    property alias font: input.font

    property int rowSpan: 1 // just offset
    property int colSpan: 1 // real width
    width: cellWidth + (cellWidth + colSpacing) * (colSpan - 1)
    height: cellHeight

    Text {
        id: input
        text: value.toFixed(app.visibleDecimals)
        anchors.right: parent.right
        anchors.rightMargin: 3
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter

        width: cellWidth
        height: cellHeight
        anchors.top: parent.top
        anchors.topMargin: (cellHeight + rowSpacing) * (rowSpan - 1) / 2
    }
}
