// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import ROWarning 1.0

Image {
    id: warningIcon

    property ROWarning warning
    property int type: warning.type

    height: 15
    width: 15 // : 0

    source: typeToIcon()
    smooth: true
    visible: warning.enabled

    function typeToIcon() {
        switch(type) {
        case ROWarning.WarningCritical: return "../../images/warning_critical1.png";
        case ROWarning.WarningCaution: return "../../images/warning_caution1.png";
        case ROWarning.WarningInfo: return "../../images/warning_info.png";
        default: return "";
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onHoveredChanged: {

            if (containsMouse) {
                mainWindow.autoPopup(popupComponent, warningIcon)//-warningIcon.height
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
                    text: warning.what
                }
            }
        }


    }
}
