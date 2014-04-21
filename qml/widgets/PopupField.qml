// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

MouseArea {
    id: popupArea
    enabled: popupLoader.status != Loader.Null /*children.length > 0*/
    hoverEnabled: modal
    anchors.fill: parent
    property alias popupComponent: popupLoader.sourceComponent
    property alias popupItem: popupLoader.item
    property bool modal: false

    function hidePopup() {
        popupLoader.sourceComponent = undefined
        modal = false
    }

    Rectangle {
//        z: -1
        anchors.fill: parent
        color: "grey"
        opacity: 0.5
        enabled: popupArea.modal && parent.enabled
        visible: popupArea.modal && enabled
//        enabled: popupField.enabled
    }

        Loader {
            id: popupLoader
            onLoaded: {
                item.forceActiveFocus()
            }

            MouseArea {
                id: stealingClickArea
                enabled: popupLoader.status == Loader.Ready
                anchors.fill: popupLoader.item
                hoverEnabled: enabled
            }
        }


}
