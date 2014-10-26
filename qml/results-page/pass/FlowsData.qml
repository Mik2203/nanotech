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
            property ROFlow flow: modelData.flow
            TableRowValue { value: modelData.title }
            TableRowValue { value: app.units.convertFlowUnits(flow.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits) }
            TableRowValue { value: app.units.convertPressureUnits(flow.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
            TableRowValue { value: app.units.convertPressureUnits(flow.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits) }
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
    }
}
