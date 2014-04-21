import QtQuick 1.1
import "widgets" as ROWidgets


Item {
    id: container
    property int elementsCount
    property int selectedIndex
    property int elementsCountMax
    property int elementsCountMin

    property bool deletable: true

    signal add(int index)
    signal remove(int index)
    signal select(int index)

    height: 40
    width: mainRow.width
    property alias label: textLabel.text

    Row {
        id: mainRow
        spacing: 2
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: labelRect
            color: "white"
            width: textLabel.width + 10
            height: textLabel.height
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: textLabel
                anchors.centerIn: parent
            }
        }

        Repeater {
            id: line
            model: container.elementsCount

            delegate:
                ROWidgets.Button {
                id: elementButton

                height: 20
                radius: 4
                width: toggled ? 50 : 40
                border.color: "lightgrey"

                onClicked: { parent.forceActiveFocus();  select(index); }
                toggled: selectedIndex == index

                text: index+1
                textFont.bold: true
                enabledTextColor: "grey"
                textHorizontalAlignment: Text.Center


                ROWidgets.BasicButton {
                    id: copyButton
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    width: height
                    height: 15
                    visible: /*container.deletable &&*/ elementButton.toggled && elementsCount < elementsCountMax


                    tooltip: app.translator.emptyString + qsTr("Copy")

                    onClicked: {
                        container.add(container.selectedIndex)
                        select(elementsCount-1)
                    }

                    Image {
                        source: copyButton.hovered ? "../images/copy1-hover.png" : "../images/copy1.png"
                        smooth: true
                        anchors.fill: parent
                    }
                }

                ROWidgets.BasicButton {
                    id: removeButton
                    anchors.right: parent.right
                    anchors.rightMargin: 2 //removeButton.containsMouse ? 0: 2
                    anchors.verticalCenter: parent.verticalCenter
                    width: height
                    height: 15 //removeButton.containsMouse ? 20: 15
                    visible: container.deletable && elementButton.toggled && elementsCount > elementsCountMin

                    tooltip: app.translator.emptyString + qsTr("Remove")

                    onClicked: {
                        container.remove(selectedIndex)
                        select(Math.min(selectedIndex, elementsCount-1))
                    }

                    Image {
                        source: removeButton.containsMouse ? "../images/delete1.png" : "../images/delete1-hover.png"
                        smooth: true
                        anchors.fill: parent
                    }
                }

            }
        }

        ROWidgets.Button {
            id: newButton
            height: 20
            width: 40
            radius: 4
            border.color: "lightgrey"
            visible: /*container.deletable &&*/ elementsCount < elementsCountMax
            text: "+"
            textFont.bold: true
            enabledTextColor: "grey"
            textHorizontalAlignment: Text.Center
            onClicked: {
                parent.forceActiveFocus();
                container.add(-1)
                select(elementsCount-1)
            }
        }
    }

}
