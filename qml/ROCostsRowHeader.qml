// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets

ROWidgets.BasicButton {
    width: rowHeaderWidth
    height: 20
    property alias text: label.text
    property alias font: label.font

    Text {
        id: label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideRight
    }

    tooltip: label.truncated ? text: ""
}
