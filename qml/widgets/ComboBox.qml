// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

ComboBoxButton {
    id: buttonContainer
    property string initialText: app.translator.emptyString + qsTr("Select...")
    property int selectedIndex: -1
    property string selectedText: app.translator.emptyString + model[selectedIndex]
    property real listViewHeightMax: height * 5

    // PARAMS TO PASS TO LISTVIEW
    property variant model
    property Component itemDelegate: defaultItemDelegate
    property Item window: mainWindow


    text: selectedIndex < 0 ? initialText : selectedText
    textFont.italic: selectedIndex < 0
    enabledTextColor: (selectedIndex < 0) ? "grey" : "black"

    signal select(int selectIndex)

    onClicked: expanded ? contract() : expand();


    Keys.onPressed: {
        if ((event.key == Qt.Key_Enter ||
             event.key == Qt.Key_Return ||
             event.key == Qt.Key_Down) && !expanded) {
            expand()
        }
    }


    Component {
        id: defaultItemDelegate
        DefaultListViewDelegate {
            width: buttonContainer.width
            height: buttonContainer.height
            text: model.modelData
        }
    }

    Component {
        id: listViewContainer
        ScrollableListView {
            id: internalListView
            model: buttonContainer.model
            width: buttonContainer.width
            height: listViewHeightMax
            selectedIndex: buttonContainer.selectedIndex
            onSelect: buttonContainer.select(index);
            onClose: buttonContainer.contract()
            itemDelegate: buttonContainer.itemDelegate
        }
    }

    //TODO with window functions
    function expand() {
        window.popup(listViewContainer, buttonContainer, 0, buttonContainer.height)
        window.hidePopup.connect(contract)
        expanded = true
    }

    function contract() {
        window.hidePopup.disconnect(contract)
        window.hidePopup();
        expanded = false
        buttonContainer.forceActiveFocus();
    }
}
