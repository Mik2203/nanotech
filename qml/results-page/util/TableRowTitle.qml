import QtQuick 1.1

Rectangle {
    property string title
    width: _TITLE_WIDTH
//    color: backgroundColor()
    height: _ROW_HEIGHT
    clip: true
    border.color: _BORDER_COLOR
//    visible: isVisible()

    Text {
        id: label;
        text: title //.toFixed(2);
//        color: textColor()
//        anchors.right: parent.right
//        anchors.rightMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        //visible: parent.height > 0
    }

//    function backgroundColor() {
//        if (type.toLowerCase() == "F".toLowerCase() ||
//                type.toLowerCase() == "Feed".toLowerCase()) {
//            return "#cfcfcf"
//        } else if (type.toLowerCase() == "P".toLowerCase() ||
//                   type.toLowerCase() == "Permeate".toLowerCase()) {
//            return "#b7ddee"//"#018CD1"
//        } else if (type.toLowerCase() == "C".toLowerCase() ||
//                   type.toLowerCase() == "Concentrate".toLowerCase()) {
//            return "#ffbbbb"
//        } else {
//            return "white"
//        }
//    }

//    function textColor() {
//        if (type.toLowerCase() == "F".toLowerCase() ||
//                type.toLowerCase() == "Feed".toLowerCase()) {
//            return "black"
//        } else if (type.toLowerCase() == "P".toLowerCase() ||
//                   type.toLowerCase() == "Permeate".toLowerCase()) {
//            return "blue"
//        } else if (type.toLowerCase() == "C".toLowerCase() ||
//                   type.toLowerCase() == "Concentrate".toLowerCase()) {
//            return "red"
//        } else {
//            return "black"
//        }
//    }

//    function isVisible() {
//        if ((!showFeed && (type.toLowerCase() == "F".toLowerCase() || type.toLowerCase() == "Feed".toLowerCase())) ||
//                !showPermeate && (type.toLowerCase() == "P".toLowerCase() || type.toLowerCase() == "Permeate".toLowerCase()) ||
//                !showConcentrate && (type.toLowerCase() == "C".toLowerCase() || type.toLowerCase() == "Concentrate".toLowerCase()))
//            return false
//        return true
//    }
}
