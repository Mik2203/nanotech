// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "../widgets" as ROWidgets

Grid {

    clip: true
    columns: 2
    spacing: 2

    Item {
        height: 20
        width: 50

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        ROWidgets.ComboBox {
            id: temperatureUnitsSelector
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            textVerticalAlignment: Text.AlignVCenter
            width: 40
            height: parent.height-2
            model: unitsText.temperatureUnitsTextList
            selectedIndex: app.units.temperatureUnits
            onSelect: app.units.temperatureUnits = selectIndex
            Connections {
                target: app.translator
                onCurrentLanguageChanged: temperatureUnitsSelector.model = unitsText.temperatureUnitsTextList
            }
        }
    }

    Item {
        height: 20
        width: 45

        Text {
            //text: "Flow:"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        ROWidgets.ComboBox {
            id: flowUnitsSelector
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.verticalCenter: parent.verticalCenter
            textVerticalAlignment: Text.AlignVCenter
            width: 40
            height: parent.height-2
            model: unitsText.flowUnitsTextList
            selectedIndex: app.units.flowUnits
            onSelect: app.units.flowUnits = selectIndex
        }
    }


    Item {
        height: 20
        width: 50

        Text {
            //text: "Pressure:"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }
        ROWidgets.ComboBox {
            id: pressureUnitsSelector
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            textVerticalAlignment: Text.AlignVCenter
            width: 40
            height: parent.height-2
            model:unitsText.pressureUnitsTextList
            selectedIndex: app.units.pressureUnits
            onSelect: app.units.pressureUnits = selectIndex
        }
    }

    Item {
        height: 20
        width: 45

        Text {
            //text: "Flux:"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }
        ROWidgets.ComboBox {
            id: fluxUnitsSelector
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.verticalCenter: parent.verticalCenter
            textVerticalAlignment: Text.AlignVCenter
            width: 40
            height: parent.height-2
            model: unitsText.fluxUnitsTextList
            selectedIndex: app.units.fluxUnits
            onSelect: app.units.fluxUnits = selectIndex
        }
    }
}
