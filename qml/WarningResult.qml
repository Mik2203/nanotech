// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import ROWarning 1.0

Row {
    id: warningIcon

    property ROWarning warning
    property int type: warning.type

    visible: warning.enabled
    height: 100
    Image {
        id: wImg
        source: typeToIcon()
        smooth: true
        anchors.verticalCenter: parent.verticalCenter

        function typeToIcon() {
            switch(type) {
            case ROWarning.WarningCritical: return "../images/warning_critical.png";
            case ROWarning.WarningCaution: return "../images/warning_caution.png";
            case ROWarning.WarningInfo: return "../images/warning_info.png";
            }
        }
    }

    Text {
        id: wMsg
        text: warning.what
        anchors.verticalCenter: parent.verticalCenter
    }
}
