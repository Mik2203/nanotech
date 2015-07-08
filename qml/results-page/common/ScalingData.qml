import QtQuick 1.1

import ROUnits 1.0
import ROFlow 1.0
import ROSolutes 1.0

import "../util"

Row {
    property variant flows

    // data
    Repeater {
        model: flows
        Column {
            property string title: modelData.title
            property ROFlow flow: modelData.flow

            TableRowValue { value: title }
            TableRowValue { value: flow.solutes.lsi }
            TableRowValue { value: flow.solutes.sdsi }
            TableRowValue { value: flow.solutes.ionicStrength }
            Repeater {
                model: ROSolutes.TotalIons
                TableRowValue {
                    visible: sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index)
                    value: flow.solutes.saturation(index) * 100.0
                }
            }
        }
    }
}
