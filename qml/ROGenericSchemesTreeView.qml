// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets


Column {
    id: solutesTreeViewRect
    property int selectedGroupIndex: 0
    spacing: 10

    Flow {
        id: groupsPanel
        spacing: 5
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

    ROWidgets.SimpleListView {
        id: schemesView
        width: 200
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
