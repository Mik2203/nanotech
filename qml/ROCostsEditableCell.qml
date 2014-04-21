// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets

Item {
    id: container
    property alias value: input.value
    property alias editable: input.editable
    property int rowSpan: 1 // just offset
    property int colSpan: 1 // real width
    signal inputChanged(variant changedValue)


    width: cellWidth + (cellWidth + colSpacing) * (colSpan - 1)
    height: cellHeight
//    height: (cellHeight + rowSpacing) * (rowSpan - 1) + cellHeight

    ROWidgets.DoubleInput {
        id: input
        anchors.right: parent.right
        width: cellWidth
        height: cellHeight
        anchors.top: parent.top
        anchors.topMargin: (cellHeight + rowSpacing) * (rowSpan - 1) / 2
        horizontalAlignment: TextInput.AlignRight
        color: "#caebff"
    }

    Component.onCompleted: input.inputChanged.connect(container.inputChanged)

}
