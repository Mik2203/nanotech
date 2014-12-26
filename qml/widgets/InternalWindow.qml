// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: iwnd
    width: 600//windowStack.width
    height: 320//windowStack.height
    clip: true

    property alias content: loader.sourceComponent
    property alias title: title.text

    property string popupType
    property alias popupComponent: popupField.popupComponent

    signal close()

    Rectangle {
        id: border
        anchors.fill: parent
        anchors.margins: 1
//        radius: 4
        border.color: "grey"
    }

    Item {
        id: windowStack
        anchors.margins: 10
        anchors.fill: parent
        Rectangle {
            id: wndHeader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 20

            Text {
                id: title
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 2
            }

            MouseArea {
                id: headerTouchArea
                anchors.fill: parent
                property real pressedX
                property real pressedY

                onPressed: {
                    headerTouchArea.pressedX = mouse.x
                    headerTouchArea.pressedY = mouse.y
                }

                onMousePositionChanged: {
                    if (pressed) {
                        var dx = mouse.x - headerTouchArea.pressedX
                        var dy = mouse.y - headerTouchArea.pressedY
                        iwnd.x += dx
                        iwnd.y += dy
                    }
                }

                Button {
                    text: "×"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: parent.height-2
                    height: parent.height-2
                    onClicked: iwnd.close()
                }
            }
        }


        ScrollArea {
            anchors.top: wndHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            Loader {
                id: loader
                property alias window: iwnd
                onLoaded: item.forceActiveFocus()
            }

        }
    }


    MouseArea {
        id: resizer
        width: 20
        height: width
        property real pressedX
        property real pressedY

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Text {
            anchors.right: parent.right
            //anchors.bottom: parent.bottom
            text: "»"
//            font.family: "Times New Roman"
            smooth: true
            rotation: 45
            font.pixelSize: 20
        }

        onPressed: {
            resizer.pressedX = mouse.x
            resizer.pressedY = mouse.y
        }

        onMousePositionChanged: {
            if (pressed) {
                var dx = mouse.x - resizer.pressedX
                var dy = mouse.y - resizer.pressedY
                iwnd.width += dx
                iwnd.height += dy
            }
        }
    }

    signal hidePopup()

    onHidePopup: {
        popupType = "none"
        popupField.hidePopup()
        if (parent) parent.forceActiveFocus()
    }


    function popup(component, offsetSourceItem, x, y) {
        if (popupField.popupComponent && popupType != "none") return
        popupField.popupComponent = component
        var mapper = offsetSourceItem.mapToItem(popupField, x, y);
        // TODO align to borders
        popupField.popupItem.x = mapper.x
        popupField.popupItem.y = mapper.y
        popupType = "popup"
    }


    function popupWindow(component, title, offsetSourceItem, x, y) {
        var newWindowComponent = Qt.createComponent("InternalWindow.qml")
        popup(newWindowComponent, offsetSourceItem, x, y)
        popupField.popupItem.content = component
        popupField.popupItem.title = title
        popupField.popupItem.close.connect(hidePopup)
        popupType = "window"
    }

    PopupField {
        id: popupField
        z: iwnd.z + 1
        onClicked: {
            iwnd.hidePopup()
            mouse.accepted = false
        }
    }
}
