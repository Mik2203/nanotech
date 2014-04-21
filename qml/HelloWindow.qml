import QtQuick 1.1
import "widgets" as ROWidgets
import ROTranslator 1.0

Column {
    id: dataColumn
    spacing: 10
    width: parent.window.width-30

    Component.onCompleted: {
        parent.window.height =dataColumn.height + 55
    }

    Item { // margins
        height: logo.height + 50
        width: logo.width
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            id: logo
            source: app.translator.currentLanguage == ROTranslator.RU ? "../images/nanoteh_logo_RU_horizontal.png" : "../images/nanoteh_logo_EN_horizontal.png"
            height: 50
            fillMode: Image.PreserveAspectFit
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        spacing: 10
        Text {
            text: app.translator.emptyString + qsTr("Select language:")
            anchors.verticalCenter: parent.verticalCenter
        }
        ROWidgets.ComboBox {
            id: languageBox
            height: 20
            window: dataColumn.parent.window
            model: [ "EN", "RU"] // TODO ?
            selectedIndex: app.translator.currentLanguage
            onSelect: app.translator.currentLanguage = selectIndex
        }

    }

    Text {
        text: app.translator.emptyString + qsTr("NanoTechPRO 1.0.1 from 03.10.2013")
    }

    Text {
        anchors.left: parent.left
        anchors.right: parent.right
        text: app.translator.emptyString + qsTr("Please serve the results of the calculation using the program Nanotech PRO as guideline only. \"RM Nanotech\" is not responsible for any problems occurring during operation of the designed system.")
        wrapMode: Text.WordWrap
    }

    Item {
//        width: parent.width
        anchors.left: parent.left
        anchors.right: parent.right
        height: infoCol.height
        Column {
            anchors.left: parent.left
            Text {
                text: app.translator.emptyString + qsTr("JSC \"RM Nanotech\"")
                anchors.left: parent.left
            }
            Text {
                text: app.translator.emptyString + qsTr("Technical support centre")
                anchors.left: parent.left
            }

        }
        Column {
            id: infoCol
            anchors.right: parent.right
            Text {
                text: "+7(4922)475-465"
                anchors.right: parent.right
            }
            Text {
                text: "+7(4922)475-479"
                anchors.right: parent.right
            }
            Text {
                text: "ctp@rmnanotech.ru"
                anchors.right: parent.right
            }
        }
    }

    Row {
        spacing: 10
        x: 2
        ROWidgets.CheckBox {
            id: showMessageBox
            checked: app.showHelloMessage
            onToggle: app.showHelloMessage = state
        }
        Text {
            text: app.translator.emptyString + qsTr("Show this message on start up")
        }
    }

    ROWidgets.Button {
        text: app.translator.emptyString + qsTr("Continue")
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked:  hidePopup()
    }
}
