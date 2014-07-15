// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

//Rectangle {
Button {
    id: container
    property bool expanded: false//container.toggled
    textHorizontalAlignment: Text.AlignLeft
    textVerticalAlignment: Text.AlignVCenter
    textLeftMargin: 3
    textRightMargin: 11

    Image {
        id: dropdownIcon
        source: "../../images/spinbox_down.png"
        rotation: expanded ? 180 : 0
        Behavior on rotation { NumberAnimation { duration: 300 } }
        smooth: true
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.verticalCenter: parent.verticalCenter
        width: 7
        height: 5
    }

}
