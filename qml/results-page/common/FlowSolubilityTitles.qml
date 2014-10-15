import QtQuick 1.1

import ROUnits 1.0
import ROFlow 1.0
import ROSolutes 1.0

import "../util"

Column {
    // headers
    Item { // spacer
        height: _ROW_HEIGHT
        width: _TITLE_WIDTH + _UNITS_WIDTH
    }
    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("pH") }
        TableRowUnits { units: "" }
    }
    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("TDS") }
        TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
    }

    // ions
    Repeater {
        model: ROSolutes.TotalIons

        Row {
            visible: sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index)
            TableRowTitle { title: app.translator.emptyString + _SOLUTE_MODEL.shortNameByIndex(index) }
            TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
        }
    }
}
