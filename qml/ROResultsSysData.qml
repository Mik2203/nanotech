// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import ROStage 1.0
import ROSolutes 1.0
import ROUnits 1.0

Column { // SYSTEM
    ROResultText { value: sys.activeArea }
    ROResultText { value: app.units.convertFluxUnits(sys.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits); }
    ROResultText { value: sys.recovery * 100.0 }
    ROResultText { value: app.units.convertFlowUnits(sys.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "F"; }
    ROResultText { value: app.units.convertFlowUnits(sys.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "P"; }
    ROResultText { value: app.units.convertFlowUnits(sys.concentrate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "C"; }
    ROResultText { value: app.units.convertPressureUnits(sys.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
    ROResultText { value: app.units.convertPressureUnits(sys.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
    ROResultText { value: app.units.convertPressureUnits(sys.concentrate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }
    ROResultText { value: app.units.convertPressureUnits(sys.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
    ROResultText { value: app.units.convertPressureUnits(sys.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
    ROResultText { value: app.units.convertPressureUnits(sys.concentrate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }

    Item { // SOLUTES HEADER SPACER
        //width: 30
        width: colWidthData
        height: rowHeight + solutesHeaderOffset
    }

    Column { // SOLUTES
        ROResultText { value: sys.feed.pH; type: "F"; }
        ROResultText { value: sys.permeate.pH; type: "P"; }
        ROResultText { value: sys.concentrate.pH; type: "C"; }
        ROResultText { value: sys.feed.solutes.totalValueMgl; type: "F"; }
        ROResultText { value: sys.permeate.solutes.totalValueMgl; type: "P"; }
        ROResultText { value: sys.concentrate.solutes.totalValueMgl; type: "C"; }

        Component {
            id: passSolutesDelegate

            Column {
                id: soluteCell
                visible: showFieldNum >0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))

                Connections {
                    target: sys.feed.solutes
                    onSolutesChanged: soluteCell.visible= showFieldNum >0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))
                }

                ROResultText {
                    id: feedSoluteText
                    value: sys.feed.solutes.mgl(index);
                    type: "F"
                    Connections {
                        target: sys.feed.solutes
                        onSolutesChanged: feedSoluteText.value= sys.feed.solutes.mgl(index);
                    }
                }

                ROResultText {
                    id: permeateSoluteText
                    value: sys.permeate.solutes.mgl(index);
                    type: "P"
                    Connections {
                        target: sys.permeate.solutes
                        onSolutesChanged: permeateSoluteText.value= sys.permeate.solutes.mgl(index);
                    }
                }

                ROResultText {
                    id: concentrateSoluteText
                    value: sys.concentrate.solutes.mgl(index);
                    type: "C"
                    Connections {
                        target: sys.concentrate.solutes
                        onSolutesChanged: concentrateSoluteText.value= sys.concentrate.solutes.mgl(index);
                    }
                }
            }

        }

        Repeater {
            model: ROSolutes.TotalIons
            delegate: passSolutesDelegate
        }
    }

    Item { // SCALING HEADER SPACER
        //width: 30
        width: colWidthData
        height: rowHeight + solutesHeaderOffset
    }

    ROResultText { value: sys.feed.solutes.lsi; type: "F"; }
    ROResultText { value: sys.permeate.solutes.lsi; type: "P"; }
    ROResultText { value: sys.concentrate.solutes.lsi; type: "C"; }

    ROResultText { value: sys.feed.solutes.sdsi; type: "F"; }
    ROResultText { value: sys.permeate.solutes.sdsi; type: "P"; }
    ROResultText { value: sys.concentrate.solutes.sdsi; type: "C"; }

    ROResultText { value: sys.feed.solutes.ionicStrength; type: "F"; }
    ROResultText { value: sys.permeate.solutes.ionicStrength; type: "P"; }
    ROResultText { value: sys.concentrate.solutes.ionicStrength; type: "C"; }

    Repeater {
        model: ROSolutes.TotalCompounds
        delegate: Column {
            visible: showFieldNum >0 && (sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index))
            ROResultText {
                id: feedSaturation
                value: sys.feed.solutes.saturation(index) * 100.0;
                type: "F";
                Connections {
                    target: sys.feed.solutes
                    onSolutesChanged: feedSaturation.value = sys.feed.solutes.saturation(index) * 100.0;
                }
            }

            ROResultText {
                id: permeateSaturation
                value: sys.permeate.solutes.saturation(index) * 100.0;
                type: "P";
                Connections {
                    target: sys.permeate.solutes
                    onSolutesChanged: permeateSaturation.value = sys.permeate.solutes.saturation(index) * 100.0;
                }
            }

            ROResultText {
                id: concentrateSaturation
                value: sys.concentrate.solutes.saturation(index) * 100.0;
                type: "C";
                Connections {
                    target: sys.concentrate.solutes
                    onSolutesChanged: concentrateSaturation.value = sys.concentrate.solutes.saturation(index) * 100.0;
                }
            }
        }
    }
}
