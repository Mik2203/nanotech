import QtQuick 1.1
//import QtDesktop 0.1
import "widgets" as ROWidgets
import ROFlow 1.0
//import ROUnitConverterTemperature 1.0
import ROSoluteModel 1.0
import ROUnits 1.0


Rectangle {
    id: panel
    color: "transparent"
    property ROFlow selectedFeed: sys.partFeed(0)
    ROSoluteModel {
        id: currentSoluteModel
        solutes: selectedFeed.solutes
    }

    Connections { target: mainWindow; onSysChanged: feedBar.select(feedBar.selectedIndex >= 0 ? 0 : -1); }
    Connections { target: sys; onFeedCountChanged: {
            if (sys.partFeedIndex(selectedFeed) == -1)
                feedBar.select(0)
        }
    }

    Item {
        id: waterPageAll
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.bottomMargin: 5
        width: 300

        Column {
            id: waterParametersColumn
            spacing: 5
            width: parent.width
            Item {
                id: waterTypeSelector
                anchors.left: parent.left
//                anchors.top: parent.top
                width: parent.width
                height: 20

                Text {
                    id: waterTypeLabel
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + qsTr("Water type: ")
                }

                ROWidgets.ComboBox {
                    id: waterType
                    anchors.right: waterTypeChosenWarning.left
                    anchors.rightMargin: 3
                    anchors.left: waterTypeLabel.right
                    anchors.leftMargin: 3
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height-2

                    model: db.waterTypes
                    selectedIndex: sys.waterTypeIndex
                    selectedText: app.translator.emptyString + model.get(selectedIndex, "name")
                    itemDelegate: ROWidgets.DefaultListViewDelegate {
                        width: buttonContainer.width
                        height: buttonContainer.height
                        text: waterType.model.get(index, "name")
                    }
                    onSelect: sys.waterTypeIndex = selectIndex

                }

                ROWidgets.WarningPopup {
                    id: waterTypeChosenWarning
                    warning: sysC.waterTypeChosen
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { // BORDER
            border.color: "grey"
            border.width: 1
            color: "transparent"
            anchors.top: feedBar.top
            anchors.topMargin: 30
            anchors.bottom: feedConf.bottom
            width: feedConf.width + 20
            x: feedBar.x-10
            z: -1
//            radius: 3
        }

        FeedBar {
            id: feedBar
            anchors.top: waterParametersColumn.bottom
            anchors.topMargin: 3
            anchors.left: parent.left
            elementsCount: sys.feedCount
            elementsCountMax: sys.MAX_FEEDS_COUNT
            elementsCountMin: sys.MIN_FEEDS_COUNT
            onAdd: sys.addPartFeed(index)
            onRemove: sys.removePartFeed(index)
            onSelect: {
                selectedIndex = index;
                if (selectedIndex >= 0) {
                    selectedFeed = sys.partFeed(selectedIndex)
                }
                else if (selectedIndex == -1) selectedFeed = sys.feed
                else if (selectedIndex == -2) selectedFeed = sys.adjustedFeed
            }
        }

        ROConfFeed {
            id: feedConf
            feedC: sysC.feedC(feedBar.selectedIndex)
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: feedBar.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            editable: feedBar.selectedIndex >= 0
            flow: selectedFeed
        }

    }

    ROWidgets.BorderPanel {
        width: 320
        anchors.top: parent.top
        anchors.topMargin: 7
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: waterPageAll.right
        anchors.leftMargin: 50

        headerLabel: app.translator.emptyString + qsTr("Scaling")

        ScalingPanel {
            id: scalingPanel
            anchors.margins: 10
            anchors.fill: parent
        }
    }
}
