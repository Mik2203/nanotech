import QtQuick 1.1

import ROSolutes 1.0

import "../util"

Row {
    Column {
        SectionTitle {
            text: app.translator.emptyString + qsTr("Streams")

            height: _ROW_HEIGHT * 2
            width: _TITLE_WIDTH + _UNITS_WIDTH
        }

        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Flow") }
            TableRowUnits { units: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits) }
        }
        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Pressure") }
            TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
        }
        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("Osmotic pressure") }
            TableRowUnits { units: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits) }
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

        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("TDS") }
            TableRowUnits { units: app.translator.emptyString + qsTr("mg/l") }
        }

        Row {
            TableRowTitle { title: app.translator.emptyString + qsTr("pH") }
            TableRowUnits { units: "" }
        }
    }
}
