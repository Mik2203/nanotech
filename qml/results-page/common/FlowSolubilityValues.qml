import QtQuick 1.1

import ROUnits 1.0
import ROFlow 1.0
import ROSolutes 1.0

import "../util"

Column {
    property string title
    property ROFlow flow

    TableRowValue { value: title }
    TableRowValue { value: flow.pH }
    TableRowValue { value: flow.solutes.totalValueMgl }
    Repeater {
        model: ROSolutes.TotalIons
        TableRowValue {
            visible: sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index)
            value: flow.solutes.mgl(index)
        }
    }
}
