import QtQuick 1.1
import ROFeed 1.0
import ROScalingElement 1.0
import "widgets" as ROWidgets

Item {
    id: container
    property alias elementsCount: line.model
    property int selectedIndex
    property int elementsCountMax
    property int elementsCountMin

    signal add(int index)
    signal remove(int index)
    signal select(int index)

    height: mainRow.height
    width: mainRow.width

    Row {
        id: mainRow
        spacing: 2


        Rectangle {
            id: labelRect
            color: "white"
            width: feedLabel.width + 10
            height: feedLabel.height
            anchors.top: parent.top
            anchors.topMargin: 24

            Text {
                id: feedLabel
                text: app.translator.emptyString + qsTr("Feed:");
                anchors.centerIn: parent
            }
        }

        Column {
            spacing: 2

            Row {
                id: elementsRow
                spacing: 2

                Repeater {
                    id: line
                    model: container.elementsCount

                    delegate: Column {
                        id: delegateContainer
                        spacing: 2

                        ROWidgets.DoubleInput {
                            id: partEditor
                            height: 20
                            radius: 4
                            width: parent.width
                            horizontalAlignment: TextInput.AlignHCenter
                            value: sys.partFeed(index).part
                            onInputChanged: sys.partFeed(index).part = changedValue
                        }

                        ROWidgets.Button {
                            id: feedButton

                            height: 20
                            radius: 4
                            width: parent.width
                            border.color: "lightgrey"

                            onClicked: { parent.forceActiveFocus();  select(index); }
                            toggled: selectedIndex == index

                            text: index+1
                            enabledTextColor: "grey"
                            textHorizontalAlignment: Text.Center


                            ROWidgets.BasicButton {
                                id: copyButton
                                anchors.left: parent.left
                                anchors.leftMargin: 2
                                anchors.verticalCenter: parent.verticalCenter
                                width: height
                                height: 15
                                visible: feedButton.toggled && elementsCount < elementsCountMax

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
                                visible: feedButton.toggled && elementsCount > elementsCountMin

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
                        states: [
                            State {
                                name: "NOTSELECTED"
                                when: !feedButton.hovered && !feedButton.toggled
                                //PropertyChanges { target: feedButton; color: "white"}
                                PropertyChanges { target: delegateContainer; width: 40 }
                                PropertyChanges { target: feedButton; textFont.bold: true }
                                PropertyChanges { target: feedButton; textFont.pixelSize: 12 }
                            },
                            State {
                                name: "SELECTED"
                                when: feedButton.toggled
                                //PropertyChanges { target: feedButton; color: "#018CD1"}
                                PropertyChanges { target: delegateContainer; width: 50 }
                                PropertyChanges { target: feedButton; textFont.bold: true }
                                PropertyChanges { target: feedButton; textFont.pixelSize: 14 }
                                //PropertyChanges { target: feedButton; enabledTextColor: "white" }
                            },
                            State {
                                name: "COVER";  when: feedButton.hovered && !feedButton.toggled
                                //PropertyChanges { target: feedButton; color: "#caebff" }
                                PropertyChanges { target: delegateContainer; width: 40 }
                                PropertyChanges { target: feedButton; textFont.bold: true }
                                PropertyChanges { target: feedButton; textFont.pixelSize: 14 }
                            }
                        ]
                        transitions: [
                            Transition {
                                from: "NOTSELECTED"; to: "SELECTED"
                                ColorAnimation { target: feedButton; duration: 150}
                                NumberAnimation { target: feedButton; properties: "width"; duration: 150}
                                PropertyAnimation { target: feedButton; properties: "textFont.pixelSize"; duration: 150}
                            },
                            Transition {
                                from: "SELECTED"; to: "NOTSELECTED"
                                ColorAnimation { target: feedButton; duration: 150}
                                NumberAnimation { target: feedButton; properties: "width"; duration: 150}
                                PropertyAnimation { target: feedButton; properties: "textFont.pixelSize"; duration: 150}
                            },
                            Transition {
                                from: "NOTSELECTED"; to: "COVER"
                                ColorAnimation { target: feedButton; duration: 150}
                                //NumberAnimation { target: element; properties: "width"; duration: 150}
                                PropertyAnimation { target: feedButton; properties: "textFont.pixelSize"; duration: 150}
                            },
                            Transition {
                                from: "COVER"; to: "NOTSELECTED"
                                ColorAnimation { target: feedButton; duration: 150}
                                //NumberAnimation { target: element; properties: "width"; duration: 150}
                                PropertyAnimation { target: feedButton; properties: "textFont.pixelSize"; duration: 150}
                            }
                        ]
                    }
                }
            }

            ROWidgets.Button {
                id: totalButton
                height: 20
                width: elementsRow.width
                toggled: selectedIndex == -1
                radius: 4
                border.color: "lightgrey"
                text: app.translator.emptyString + qsTr("Total")
                enabledTextColor: "grey"
                textHorizontalAlignment: Text.Center
                onClicked: select(-1)

                states: [
                    State {
                        name: "NOTSELECTED"
                        when: !totalButton.hovered && !totalButton.toggled
                        //PropertyChanges { target: totalButton; color: "white"}
                        //PropertyChanges { target: delegateContainer; width: 40 }
                        PropertyChanges { target: totalButton; textFont.bold: true }
                        PropertyChanges { target: totalButton; textFont.pixelSize: 12 }

                    },
                    State {
                        name: "SELECTED"
                        when: totalButton.toggled
                        //PropertyChanges { target: totalButton; color: "#018CD1"}
                        //PropertyChanges { target: delegateContainer; width: 50 }
                        PropertyChanges { target: totalButton; textFont.bold: true }
                        PropertyChanges { target: totalButton; textFont.pixelSize: 14 }
                        //PropertyChanges { target: totalButton; enabledTextColor: "white"}
                    },
                    State {
                        name: "COVER";  when: totalButton.hovered && !totalButton.toggled
                        //PropertyChanges { target: totalButton; color: "#caebff" }
                        //PropertyChanges { target: delegateContainer; width: 40 }
                        PropertyChanges { target: totalButton; textFont.bold: true }
                        PropertyChanges { target: totalButton; textFont.pixelSize: 14 }
                    }
                ]
                transitions: [
                    Transition {
                        from: "NOTSELECTED"; to: "SELECTED"
                        ColorAnimation { target: totalButton; duration: 150}
                        //NumberAnimation { target: totalButton; properties: "width"; duration: 150}
                        PropertyAnimation { target: totalButton; properties: "textFont.pixelSize"; duration: 150}
                    },
                    Transition {
                        from: "SELECTED"; to: "NOTSELECTED"
                        ColorAnimation { target: totalButton; duration: 150}
                        //NumberAnimation { target: totalButton; properties: "width"; duration: 150}
                        PropertyAnimation { target: totalButton; properties: "textFont.pixelSize"; duration: 150}
                    },
                    Transition {
                        from: "NOTSELECTED"; to: "COVER"
                        ColorAnimation { target: totalButton; duration: 150}
                        //NumberAnimation { target: element; properties: "width"; duration: 150}
                        PropertyAnimation { target: totalButton; properties: "textFont.pixelSize"; duration: 150}
                    },
                    Transition {
                        from: "COVER"; to: "NOTSELECTED"
                        ColorAnimation { target: totalButton; duration: 150}
                        //NumberAnimation { target: element; properties: "width"; duration: 150}
                        PropertyAnimation { target: totalButton; properties: "textFont.pixelSize"; duration: 150}
                    }
                ]
            }

            ROWidgets.Button {
                id: adjTotalButton
                height: 20
                width: elementsRow.width
                toggled: selectedIndex == -2
                radius: 4
                border.color: "lightgrey"
                text: app.translator.emptyString + qsTr("Adj")
                enabledTextColor: "grey"
                textHorizontalAlignment: Text.Center
                onClicked: select(-2)
                visible: sys.scalingElement.adjustment != ROScalingElement.NoAdjusment

                states: [
                    State {
                        name: "NOTSELECTED"
                        when: !adjTotalButton.hovered && !adjTotalButton.toggled
                        //PropertyChanges { target: adjTotalButton; color: "white"}
                        //PropertyChanges { target: delegateContainer; width: 40 }
                        PropertyChanges { target: adjTotalButton; textFont.bold: true }
                        PropertyChanges { target: adjTotalButton; textFont.pixelSize: 12 }

                    },
                    State {
                        name: "SELECTED"
                        when: adjTotalButton.toggled
                        //PropertyChanges { target: adjTotalButton; color: "#018CD1"}
                        //PropertyChanges { target: delegateContainer; width: 50 }
                        PropertyChanges { target: adjTotalButton; textFont.bold: true }
                        PropertyChanges { target: adjTotalButton; textFont.pixelSize: 14 }
                        //PropertyChanges { target: adjTotalButton; enabledTextColor: "white"}
                    },
                    State {
                        name: "COVER";  when: adjTotalButton.hovered && !adjTotalButton.toggled
                        //PropertyChanges { target: adjTotalButton; color: "#caebff" }
                        //PropertyChanges { target: delegateContainer; width: 40 }
                        PropertyChanges { target: adjTotalButton; textFont.bold: true }
                        PropertyChanges { target: adjTotalButton; textFont.pixelSize: 14 }
                    }
                ]
                transitions: [
                    Transition {
                        from: "NOTSELECTED"; to: "SELECTED"
                        ColorAnimation { target: adjTotalButton; duration: 150}
                        //NumberAnimation { target: adjTotalButton; properties: "width"; duration: 150}
                        PropertyAnimation { target: adjTotalButton; properties: "textFont.pixelSize"; duration: 150}
                    },
                    Transition {
                        from: "SELECTED"; to: "NOTSELECTED"
                        ColorAnimation { target: adjTotalButton; duration: 150}
                        //NumberAnimation { target: adjTotalButton; properties: "width"; duration: 150}
                        PropertyAnimation { target: adjTotalButton; properties: "textFont.pixelSize"; duration: 150}
                    },
                    Transition {
                        from: "NOTSELECTED"; to: "COVER"
                        ColorAnimation { target: adjTotalButton; duration: 150}
                        //NumberAnimation { target: element; properties: "width"; duration: 150}
                        PropertyAnimation { target: adjTotalButton; properties: "textFont.pixelSize"; duration: 150}
                    },
                    Transition {
                        from: "COVER"; to: "NOTSELECTED"
                        ColorAnimation { target: adjTotalButton; duration: 150}
                        //NumberAnimation { target: element; properties: "width"; duration: 150}
                        PropertyAnimation { target: adjTotalButton; properties: "textFont.pixelSize"; duration: 150}
                    }
                ]
            }
        }

        Column {
            spacing: 2

            Text {
                text: "%"//app.translator.emptyString + qsTr("part")
                font.italic: true
                height: 20
                width: newButton.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            ROWidgets.Button {
                id: newButton
                height: 20
                width: 40
                radius: 4
                border.color: "lightgrey"
                visible: elementsCount < elementsCountMax
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
    //}
}
