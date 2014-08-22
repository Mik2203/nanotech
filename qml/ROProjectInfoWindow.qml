// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets

Rectangle {
    width: 300
    height: layout.height + layout.anchors.topMargin + layout.anchors.bottomMargin
    border.color: "lightgray"
    color: "white"

    property real rowHeight: 20
    property real paramNameWidth: 80

    Column {
        id: layout
        anchors.margins: 4
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

        Text {
            text: qsTr("Project info")
            font.bold: true
        }

        Item {
            id: nameEditorRow
            anchors.left: parent.left
            anchors.right: parent.right
            //            anchors.top: parent.top
            height: rowHeight

            Text {
                text: app.translator.emptyString + qsTr("Name: ")
                id: nameLabel
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: paramNameWidth
            }

            ROWidgets.TextInput {
                anchors.right: parent.right
                anchors.left: nameLabel.right
                anchors.leftMargin: 5

                value: projInfo.name
                onInputChanged: projInfo.name = changedValue
            }
        }

        Item {
            id: authorEditorRow
            anchors.left: parent.left
            anchors.right: parent.right
            //            anchors.top: nameEditorRow.bottom
            //            anchors.topMargin: 5
            height: rowHeight

            Text {
                text: app.translator.emptyString + qsTr("Author: ")
                id: authorLabel
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: paramNameWidth
            }

            ROWidgets.TextInput {
                anchors.right: parent.right
                anchors.left: authorLabel.right
                anchors.leftMargin: 5

                value: projInfo.author
                onInputChanged: projInfo.author = changedValue
            }
        }

        Item {
            id: companyEditorRow
            anchors.left: parent.left
            anchors.right: parent.right
            //            anchors.top: authorEditorRow.bottom
            //            anchors.topMargin: 5
            height: rowHeight

            Text {
                text: app.translator.emptyString + qsTr("Company: ")
                id: companyLabel
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: paramNameWidth
            }

            ROWidgets.TextInput {
                anchors.right: parent.right
                anchors.left: companyLabel.right
                anchors.leftMargin: 5

                value: projInfo.company
                onInputChanged: projInfo.company = changedValue
            }
        }

        Item {
            id: notesEditorRow
            anchors.left: parent.left
            anchors.right: parent.right
            //            anchors.top: companyEditorRow.bottom
            //            anchors.topMargin: 5
            //            anchors.bottom: parent.bottom
            //height: rowHeight
            height: 4 * rowHeight // TODO columnlayout

            Text {
                text: app.translator.emptyString + qsTr("Notes: ")
                id: notesLabel
                anchors.left: parent.left
                anchors.top: parent.top
                //anchors.verticalCenter: parent.verticalCenter
                width: paramNameWidth
            }

            ROWidgets.TextEdit {
                anchors.right: parent.right
                anchors.left: notesLabel.right
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                value: projInfo.notes
                onInputChanged: projInfo.notes = changedValue
            }
        }
    }


    //}

}
