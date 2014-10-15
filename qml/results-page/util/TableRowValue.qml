import QtQuick 1.1

Rectangle {
    property variant value
    width: _VALUE_WIDTH
    height: _ROW_HEIGHT
    border.color: _BORDER_COLOR
    clip: true;

    function _formatValue(value) {
        if (typeof(value) != "number")
            return value;

        if (value == parseInt(value))
            return value;

        return value.toFixed(2);
    }

    Text {
        text: _formatValue(value) // .toFixed(2);
        anchors.right: parent.right;
        anchors.rightMargin: 5;
        anchors.verticalCenter: parent.verticalCenter
    }
}
