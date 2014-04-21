import QtQuick 1.1

Item {
    id:tab
    anchors.fill: parent
    anchors.margins: 10
    property string title
    property string imageSource
    property real imageWidth: 20
    property real imageHeight: 20
    property bool active: true
}
