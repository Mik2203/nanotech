// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import ROWarning 1.0

Image {
    id: warningIcon

    //property variant warnings
    property string text
    height: 15
    width: 15 // : 0
    source: "../images/warning_caution1.png"
    smooth: true
    visible: text.length > 0

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onHoveredChanged: {
            if (containsMouse) {
                mainWindow.autoPopup(popupComponent, warningIcon)
            } else if (mainWindow.popupComponent == popupComponent) {
                mainWindow.hidePopup()
            }
        }

        Component {
            id: popupComponent

            Rectangle {
                id: popup
                width: popupLabel.width + 5
                height: popupLabel.height + 5
                border.color: "lightgrey"
                border.width: 1

                Text {
                    id: popupLabel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    text: warningIcon.text
                }
            }
        }


    }
}
