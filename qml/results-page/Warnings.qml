// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import ROSystemSolveScheduler 1.0
import ROPassController 1.0
import ROStageController 1.0
import ROElementController 1.0
import ROSolutes 1.0
import ROWarning 1.0

Column {

    spacing: 20

    Row {

        spacing: 20

        visible: sysC.hasAnyCriticalWarnings
        Image {
            id: criticalImg
            source: "../../images/warning_critical.png"
            smooth: true
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: criticalText
            anchors.verticalCenter: parent.verticalCenter
            text: sysC.allWarningsMessages(ROWarning.WarningCritical).join("\n")

            Connections {
                target: sysC
                onHasAnyCriticalWarningsChanged: criticalText.text = sysC.allWarningsMessages(ROWarning.WarningCritical).join("\n")
            }
            Connections {
                target: mainWindow
                onCase_Changed: criticalText.text = sysC.allWarningsMessages(ROWarning.WarningCritical).join("\n")
            }
        }
    }


    Row {
        spacing: 20

        visible: (!sysC.hasAnyCriticalWarnings && !sysSS.calculated) || sysSS.solved
        Image {
            id: infoImg
            source: "../../images/warning_info.png"
            smooth: true
            anchors.verticalCenter: parent.verticalCenter
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            // SYSTEM
            Text { visible: !sysC.hasAnyCriticalWarnings && !sysSS.calculated && !sysSS.calculating; text: app.translator.emptyString + qsTr("System is ready to be calculated") }
            Text { visible: sysSS.status & ROSystemSolveScheduler.Calculating; text: app.translator.emptyString + qsTr("System is calculating. Please wait.") }
            Text { visible: sysSS.solved; text: app.translator.emptyString + qsTr("System has been calculated") }
            Text { visible: (sysSS.calculated || sysSS.calculating) && sysSS.needUpdate; text: app.translator.emptyString + qsTr("System has been changed. Recalculate.") }
        }
    }

    Row {
        spacing: 20
        visible: sysC.hasAnyCautionWarnings && !sysC.hasAnyCriticalWarnings

        Image {
            id: cautionImg
            source: "../../images/warning_caution.png"
            smooth: true
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: cautionText
            anchors.verticalCenter: parent.verticalCenter
            text: sysC.allWarningsMessages(ROWarning.WarningCaution).join("\n")
            Connections {
                target: sysC
                onHasAnyCautionWarningsChanged: cautionText.text = sysC.allWarningsMessages(ROWarning.WarningCaution).join("\n")
            }
            Connections {
                target: mainWindow
                onCase_Changed: cautionText.text = sysC.allWarningsMessages(ROWarning.WarningCaution).join("\n")
            }
        }

    }
}
