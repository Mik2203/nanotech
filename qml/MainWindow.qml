// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets
import "MenuPanel"

import ROSystem 1.0
import ROSystemController 1.0
import ROSystemSolver 1.0
import ROSystemSolveScheduler 1.0
import ROProject 1.0
import ROCase 1.0
import ROPass 1.0
import ROStage 1.0
import ROProjectInfo 1.0
import ROTranslator 1.0
import ROUnits 1.0

import "./results-page" as Results

ROWidgets.ExternalWindow {
    id: mainWindow
    title: "Nanotech PRO " + app.projectManager.projFileName + (app.projectManager.projWasAltered ? "*" : "")
    width: 850
    height: 500
    minimumWidth: 850
    minimumHeight: 500

    visible: true

    property ROProject proj: app.projectManager.proj
    property ROCase case_: proj.firstCase
    property ROProjectInfo projInfo: proj.info
    property ROSystem sys: case_.sys
    property ROSystemSolveScheduler sysSS: case_.sysSS
    property ROSystemSolver sysS: sysSS.solver
    property ROSystemController sysC: case_.sysC
    property ROPass selectedPass: sys.firstPass
    property ROStage selectedStage: selectedPass.firstStage

    // Обработчики изменений, которые необходимо отслеживать вручную
    onSysChanged: selectedPass = sys.firstPass
    Connections { target: app.projectManager; onProjChanged: case_ = proj.firstCase }
    Connections { target: proj; onCaseCountChanged: { if (proj.caseIndex(case_) === -1) case_ = proj.firstCase } }
    Connections { target: sys; onPassCountChanged: { if (sys.passIndex(selectedPass) === -1) selectedPass = sys.firstPass } } // undefined, reselect
    Connections { target: selectedPass; onStageCountChanged: { if (selectedPass.stageIndex(selectedStage) === -1) selectedStage = selectedPass.firstStage } } // undefined, reselect

    onBeforeClosed: closeByUser = app.projectManager.maybeSave();

    Component.onCompleted: {
        if (app.showHelloMessage)
            mainWindow.popupWindow(Qt.createComponent("HelloWindow.qml"), app.translator.emptyString + qsTr("Hello!"), mainWindow, undefined, undefined, true)
    }


    MenuPanel {
        id: menuPanel
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
    }

    Image {
        id: background
        z: -1
        source: "../images/background2.png"
        rotation: 180
        width: 400
        fillMode: Image.PreserveAspectFit
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }


    Image {
        id: background2
        z: -1
        source: "../images/background2.png"
        anchors.top: parent.top
        anchors.left: parent.left
        fillMode: Image.PreserveAspectFit
        width: 400

    }


    ROWidgets.TabFrame {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: menuPanel.bottom
        anchors.bottom: parent.bottom

        anchors.margins: 10



        ROWidgets.Tab {
            title: app.translator.emptyString + qsTr("Feedwater")
            imageSource: "../../images/feedwater.png"
            active: !sysSS.calculating
            FeedWaterPage {
                anchors.fill: parent
            }
        }

        ROWidgets.Tab {
            title: app.translator.emptyString + qsTr("System")
            imageSource: "../../images/system.png"
            active: !sysSS.calculating
            ROConfSystem {
                anchors.fill: parent
            }
        }

        ROWidgets.Tab {
            title: app.translator.emptyString + qsTr("Results")
            imageWidth: 16
            imageHeight: 16
            imageSource: {
                if (sysC.hasAnyCriticalWarnings) return "../../images/warning_critical1.png"
                if (sysC.hasAnyCautionWarnings) return "../../images/warning_caution1.png"
                if (sysSS.solved) return "../../images/results-done.png"
                return "../../images/warning_info1.png"
            }
            Results.Tab {
                anchors.fill: parent
            }
        }

        ROWidgets.Tab {
            title: app.translator.emptyString + qsTr("Costs")
            imageSource: "../../images/costs.png"
            active: sysSS.solved
            ROCosts {
                anchors.fill: parent
            }
        }
    }

}

