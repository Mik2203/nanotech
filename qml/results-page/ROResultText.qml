// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property real value
    property string type: ""
    width: colWidthData
    color: backgroundColor()
    height: rowHeight
    clip: true
    border.color: borderColor
    visible: isVisible()

    Text {
        id: label;
        text: value.toFixed(2);
        color: textColor()
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        //visible: parent.height > 0
    }

    function backgroundColor() {
        if (type.toLowerCase() == "F".toLowerCase() ||
                type.toLowerCase() == "Feed".toLowerCase()) {
            return "#cfcfcf"
        } else if (type.toLowerCase() == "P".toLowerCase() ||
                   type.toLowerCase() == "Permeate".toLowerCase()) {
            return "#b7ddee"//"#018CD1"
        } else if (type.toLowerCase() == "C".toLowerCase() ||
                   type.toLowerCase() == "Concentrate".toLowerCase()) {
            return "#ffbbbb"
        } else {
            return "white"
        }
    }

    function textColor() {
        if (type.toLowerCase() == "F".toLowerCase() ||
                type.toLowerCase() == "Feed".toLowerCase()) {
            return "black"
        } else if (type.toLowerCase() == "P".toLowerCase() ||
                   type.toLowerCase() == "Permeate".toLowerCase()) {
            return "blue"
        } else if (type.toLowerCase() == "C".toLowerCase() ||
                   type.toLowerCase() == "Concentrate".toLowerCase()) {
            return "red"
        } else {
            return "black"
        }
    }

    function isVisible() {
        if ((!showFeed && (type.toLowerCase() == "F".toLowerCase() || type.toLowerCase() == "Feed".toLowerCase())) ||
                !showPermeate && (type.toLowerCase() == "P".toLowerCase() || type.toLowerCase() == "Permeate".toLowerCase()) ||
                !showConcentrate && (type.toLowerCase() == "C".toLowerCase() || type.toLowerCase() == "Concentrate".toLowerCase()))
            return false
        return true
    }
}
