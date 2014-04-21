// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import WheelArea 0.1

Column {
    id: container
    property alias model: listView.model
    property alias selectedIndex: listView.currentIndex
    property alias itemDelegate: listView.delegate
    property alias headerDelegate: headerLoader.sourceComponent
    width: Math.max(headerLoader.width, listView.width)

    signal select(int index);
    signal close();

    Loader {
        id: headerLoader
    }

    Repeater {
        id: listView
        focus: true
        property int currentIndex: 0

        Keys.onPressed: {
            if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
                select(listView.currentIndex)
                close();
            } else if (event.key == Qt.Key_Escape) {
                close();
            }
        }
    }
}
