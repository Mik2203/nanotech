import QtQuick 1.1

Rectangle {
    property string units
    width: _UNITS_WIDTH
    height: _ROW_HEIGHT
    border.color: _BORDER_COLOR
    clip: true;
    Text {
        text: units;
        font.italic: true;
        anchors.left: parent.left;
        anchors.leftMargin: 5;
        anchors.verticalCenter: parent.verticalCenter
    }
}
