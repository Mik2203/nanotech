// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets


Item {
    id: solutesTreeViewRect
    property int selectedGroupIndex: 0
    anchors.fill: parent

    Flow {
        id: groupsPanel
        spacing: 5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Repeater {
            model: db.genericSchemes
            delegate: ROWidgets.Button {
                height: 20
                text: group
                toggled: selectedGroupIndex == index
                onClicked: selectedGroupIndex = index
            }
        }
    }

    ROWidgets.ScrollableListView {
        id: schemesView

        anchors.top: groupsPanel.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: db.genericSchemes.schemesByIndex(selectedGroupIndex)
        itemDelegate: ROWidgets.DefaultListViewDelegate {
            width: schemesView.width-2 // dont know why -2 ...
            height: 20
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: 3
                text: modelData
            }
        }

        onSelect: {
            db.genericSchemes.loadScheme(case_, selectedGroupIndex, index)
            mainWindow.hidePopup()
        }
    }
}
