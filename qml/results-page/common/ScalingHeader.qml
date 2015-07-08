import QtQuick 1.1

import ROSolutes 1.0

import "../util"

Row {
    Column {
        SectionTitle {
            text: app.translator.emptyString + qsTr("Scaling")

            height: _ROW_HEIGHT * 2
            width: _TITLE_WIDTH + _UNITS_WIDTH
        }

        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("LSI") }
            TableRowUnits { units: "" }
        }
        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("S&DSI") }
            TableRowUnits { units: "" }
        }
        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Ionic Strength") }
            TableRowUnits { units: "" }
        }

        // compounds
        Repeater {
            model: ROSolutes.TotalCompounds

            Row {
                visible: sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index)
                TableRowTitle { title: app.translator.emptyString + _SOLUTE_MODEL.shortCompoundNameByIndex(index) }
                TableRowUnits { units: "%" }
            }
        }
    }
}
