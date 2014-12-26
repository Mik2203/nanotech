// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import WheelArea 0.1

Item {
    id: container

    property alias model: listView.model
    property alias selectedIndex: listView.currentIndex
//    property alias selectedItem: listView.currentItem//.selectedInternalItem

    property alias itemDelegate: listView.delegate
    property alias headerDelegate: headerLoader.sourceComponent

    signal select(int index);
    signal close();

    Loader {
        id: headerLoader
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    ScrollArea {
        enableHorizontalScrollBar: false // WIDTH IS SET BY HEADER!!!
        frame: false
        anchors.top: headerLoader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
//        height: listView.contentHeight
//        width: headerLoader.width

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            Repeater {
                id: listView
                anchors.left: parent.left
                anchors.right: parent.right
                property int currentIndex: 0
    //            height: contentHeight


    //            anchors.top: parent.top
    //            anchors.bottom: parent.bottom
                focus: true

                Keys.onPressed: {
                    if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
                        select(listView.currentIndex)
                        close();
                    } else if (event.key == Qt.Key_Escape) {
                        close();
                    }
                }
//                boundsBehavior: Flickable.StopAtBounds
            }
        }
    }
}
