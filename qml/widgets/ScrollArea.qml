import QtQuick 1.1
//import "custom" as Components
//import "private" as Private

FocusScope {
    id: root
    width: 100
    height: 100

    // Cosmetic propeties
    property bool frame: true
    property int frameWidth: frame ? 1 : 0

    // Item properties
    property alias horizontalScrollBar: scroller.horizontalScrollBar
    property alias verticalScrollBar: scroller.verticalScrollBar

    property bool enableHorizontalScrollBar: true
    property bool enableVerticalScrollBar: true

    // Viewport properties
    property int contentX
    property int contentY
    //Behavior on contentX { NumberAnimation { duration: 100 } }
    //Behavior on contentY { NumberAnimation { duration: 100 } }
    property int contentHeight : content.childrenRect.height
    property int contentWidth: content.childrenRect.width
    property int viewportHeight: height - (horizontalScrollBar.visible ? verticalScrollBar.barCompactWidth : 0) - 2 * frameWidth
    property int viewportWidth: width - (verticalScrollBar.visible ? verticalScrollBar.barCompactWidth : 0) - 2 * frameWidth
    default property alias data: content.data

    onContentYChanged: {
        scroller.blockUpdates = true
        verticalScrollBar.value = contentY
        scroller.verticalValue = contentY
        scroller.blockUpdates = false
    }

    onContentXChanged: {
        scroller.blockUpdates = true
        horizontalScrollBar.value = contentX
        scroller.horizontalValue = contentX
        scroller.blockUpdates = false
    }

    Item {
        id: clipper
        anchors.fill: parent
        anchors.margins: frameWidth
        clip: true
        Item {
            id: content
            x: -root.contentX
            y: -root.contentY
        }
    }


    ScrollAreaHelper { id: scroller }
}
