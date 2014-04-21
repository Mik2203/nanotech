// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets


Column {
    // TODO bring to QML waterGroupModel
    property int selectedGroupId: 0

    function getFilterCount(selectedGroupId) {
        var counter = 0;
        for (var i = 0; i < solutesIO.waterModel.count; i++) {
            var groupId = solutesIO.waterModel.get(i, "group_id");
            if (groupId == selectedGroupId) ++counter;
        }
        return counter;
    }


    id: solutesTreeViewRect
    spacing: 10

    onFocusChanged: if (focus) groupsPanel.forceActiveFocus();
//    Component.onCompleted: {
//        // выборка списка групп в groupModel TODO - СДЕЛАТЬ в db waterGroupModel!!!
//        groupIdModel = solutesIO.waterModel.waterGroupsIds;
//        groupNameModel = solutesIO.waterModel.waterGroupsNames;
//    }

    Flow {
        id: groupsPanel
        spacing: 5
        Repeater {
            model: solutesIO.waterModel.waterGroupsIds
            delegate: ROWidgets.Button {
                height: 20;
                text: solutesIO.waterModel.waterGroupsNames[index]
                toggled: selectedGroupId == modelData
                onClicked: selectedGroupId = modelData
            }
        }
    }

    ROWidgets.SimpleListView {
        id: waterView
        width: 300
        model: solutesIO.waterModel
        itemDelegate: ROWidgets.DefaultListViewDelegate {
            visible: selectedGroupId == model.group_id
            width: waterView.width-2 // dont know why -2 ...
            height: visible ? 20 : 0

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: 3
                text: model.name
            }

            ROWidgets.BasicButton {
                id: removeButton
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                width: height
                visible: model.group_id == solutesIO.waterModel.USER_GROUP_ID
                height: 15

                onClicked: solutesIO.removeFromDB(model.index)

                Image {
                    source: removeButton.containsMouse ? "../images/delete1.png" : "../images/delete1-hover.png"
                    smooth: true
                    anchors.fill: parent
                }
            }
        }

        onSelect: {
            solutesIO.loadFromDB(index, currentSoluteModel)
            solutesName.value = solutesIO.waterModel.get(index, "name")
            mainWindow.hidePopup()
        }
    }
}
