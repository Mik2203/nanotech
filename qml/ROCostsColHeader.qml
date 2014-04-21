// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets


ROWidgets.BasicButton {
    //active: false

    property alias text: label.text
    property int colSpan: 1 // real width

    width: cellWidth + (cellWidth + colSpacing) * (colSpan - 1)
    height: cellHeight

    Text {
        id: label
        anchors.right: parent.right
        anchors.left: parent.left
        elide: Text.ElideRight
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignRight
    }

    tooltip: label.truncated ? text: ""
}
