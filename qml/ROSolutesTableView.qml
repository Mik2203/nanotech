import QtQuick 1.1
import "widgets" as ROWidgets
import ROSoluteModel 1.0
import ROSolutes 1.0

Item {
    id: container
    property bool editable: true
    height: solutesView.contentHeight


    Component {
        id: soluteDelegate

        Item {
            id: delegateContainer
            height: 20
            width: container.width

            Text {
                text: label;
                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter;
                font.bold: type&ROSoluteModel.TotalRowType
            }

            ROWidgets.DoubleInput {
                id: mglInput
                focus: true
                anchors.right: meqlInput.left; anchors.rightMargin: 5;
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height-3
                width: 70
                font.bold: type&ROSoluteModel.TotalRowType
                value: mgl;
                horizontalAlignment: TextInput.AlignRight
                editable: container.editable && solutesView.model.isEditable(index)
                onInputChanged: solutesView.model.setMgl(index, changedValue)
                KeyNavigation.tab: meqlInput
                KeyNavigation.backtab: delegateContainer.KeyNavigation.backtab
            }

            ROWidgets.DoubleInput {
                id: meqlInput
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                //anchors.rightMargin: parent.height+3;
                height: parent.height-3
                width: 70
                font.bold: type&ROSoluteModel.TotalRowType
                value: meql
                editable: container.editable && solutesView.model.isEditable(index)
                horizontalAlignment: TextInput.AlignRight
                onInputChanged: solutesView.model.setMeql(index, changedValue)
                //KeyNavigation.tab: adjustButton
                KeyNavigation.backtab: mglInput
            }

        }
    }

    ListView {
        id: solutesView
        model: currentSoluteModel
        focus: true
        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds
        delegate:soluteDelegate
    }
}
