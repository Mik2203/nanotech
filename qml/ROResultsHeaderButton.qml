// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import "widgets" as ROWidgets

ROWidgets.Button {
    id: button
    width: button.toggled ? colWidthData * colNumData : colWidthData;
    height: rowHeight;
    togglable: true
    textHorizontalAlignment: Text.AlignHCenter
    radius: 0

    Image {
        id: dropdownIcon
        source: "../images/spinbox_down.png"
        rotation: button.toggled ? 90 : -90
        Behavior on rotation {
                 NumberAnimation { duration: 300 }
             }
        smooth: true
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.verticalCenter: parent.verticalCenter
        width: 7
        height: 5
    }
}
