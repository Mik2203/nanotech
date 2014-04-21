import QtQuick 1.1
import WheelArea 0.1

WheelArea {
    id: wheelarea

    property alias horizontalScrollBar: hscrollbar
    property alias verticalScrollBar: vscrollbar
    property bool blockUpdates: false
    property int availableHeight : root.height - (hscrollbar.visible ? hscrollbar.barExpandWidth : 0)
    property int availableWidth: root.width - vscrollbar.barExpandWidth
    scrollSpeed: 5.0

    anchors.fill: parent
    anchors.margins: frameWidth
    horizontalMinimumValue: hscrollbar.minimumValue
    horizontalMaximumValue: hscrollbar.maximumValue
    verticalMinimumValue: vscrollbar.minimumValue
    verticalMaximumValue: vscrollbar.maximumValue

    onVerticalValueChanged: {
        if (!blockUpdates)
            verticalScrollBar.value = verticalValue
    }

    onHorizontalValueChanged: {
        if (!blockUpdates)
            horizontalScrollBar.value = horizontalValue
    }

    ScrollBar {
        id: hscrollbar
        orientation: Qt.Horizontal
        visible: contentWidth > availableWidth && enableHorizontalScrollBar
        maximumValue: visible ? root.contentWidth - availableWidth : 0
        minimumValue: 0
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: vscrollbar.visible ? vscrollbar.barCompactWidth: 0//frameWidth
        onValueChanged: {
            if (!blockUpdates) {
                contentX = value
                horizontalValue = value
            }
        }
    }

    ScrollBar {
        id: vscrollbar
        orientation: Qt.Vertical
        visible: contentHeight > availableHeight && enableVerticalScrollBar
        maximumValue: visible ? root.contentHeight - availableHeight : 0
        minimumValue: 0
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: hscrollbar.visible ? hscrollbar.barCompactWidth: 0//frameWidth

        onValueChanged: {
            if (!blockUpdates) {
                contentY = value
                verticalValue = value
            }
        }
    }
}
