// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import Window 0.1

Window {
    id: ewnd
    width: 100
    height: 100

    property string popupType
    property alias popupComponent: popupField.popupComponent
    property alias popupItem: popupField.popupItem

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
        // set pos with align if outside of window
        popupField.popupItem.x = Math.max(Math.min(mapper.x, width - popupField.popupItem.width), 0)
        popupField.popupItem.y = Math.max(Math.min(mapper.y, height - popupField.popupItem.height), 0)
        popupType = "popup"
    }

    function autoPopup(component, offsetSourceItem) {
        if (popupField.popupComponent && popupType != "none") return
        popupField.popupComponent = component
        var mapper = offsetSourceItem.mapToItem(popupField, 0, -5-popupField.popupItem.height);
        // set pos with align if outside of window
        popupField.popupItem.x = Math.max(Math.min(mapper.x, width - popupField.popupItem.width), 0)
        popupField.popupItem.y = Math.max(Math.min(mapper.y, height - popupField.popupItem.height), 0)
        popupType = "popup"
    }

    function popupWindow(component, title, offsetSourceItem, x, y, modal) {
        var newWindowComponent = Qt.createComponent("InternalWindow.qml")
        popup(newWindowComponent, offsetSourceItem, x, y)
        popupField.popupItem.content = component
        popupField.popupItem.title = title
        popupField.popupItem.close.connect(hidePopup)
        if (modal) popupField.modal = true
        popupType = "window"
    }

    PopupField {
        id: popupField
        z: ewnd.z+1

        onPressed:{ ewnd.hidePopup();  mouse.accepted = false }
    }



    Rectangle {
        anchors.fill: parent
        color: "white"
        z: -100
    }
}

