// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


import ROPass 1.0
import ROSolutes 1.0
import ROUnits 1.0

Row {
    property ROPass pass: sys.pass(index)

    Repeater {
        model: pass.stageCount
        delegate: Loader {
            Component {
                id: stageComponent
                ROResultsStageData { }
            }

            sourceComponent: expandFlags[pass] & expandFlags[sys] ? stageComponent : nullWidthStub
        }
    }

    Column { //PASS DATA
//        Behavior on width { NumberAnimation { duration: 100 } }

        ROResultText { value: pass.activeArea }
        ROResultText { value: app.units.convertFluxUnits(pass.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits); }
        ROResultText { value: pass.recovery * 100.0; }
        ROResultText { value: app.units.convertFlowUnits(pass.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "F"; }
        ROResultText { value: app.units.convertFlowUnits(pass.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "P"; }
        ROResultText { value: app.units.convertFlowUnits(pass.concentrate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "C"; }
        ROResultText { value: app.units.convertPressureUnits(pass.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
        ROResultText { value: app.units.convertPressureUnits(pass.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
        ROResultText { value: app.units.convertPressureUnits(pass.concentrate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }
        ROResultText { value: app.units.convertPressureUnits(pass.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
        ROResultText { value: app.units.convertPressureUnits(pass.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
        ROResultText { value: app.units.convertPressureUnits(pass.concentrate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }

        Item { // SOLUTES HEADER SPACER
            //width: 30
            width: colWidthData
            height: rowHeight + solutesHeaderOffset
        }

        Column { // SOLUTES
            ROResultText { value: pass.feed.pH; type: "F"; }
            ROResultText { value: pass.permeate.pH; type: "P"; }
            ROResultText { value: pass.concentrate.pH; type: "C"; }

            ROResultText { value: pass.feed.solutes.totalValueMgl; type: "F"; }
            ROResultText { value: pass.permeate.solutes.totalValueMgl; type: "P"; }
            ROResultText { value: pass.concentrate.solutes.totalValueMgl; type: "C"; }

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
                        value: pass.feed.solutes.mgl(index);
                        type: "F"
                        Connections {
                            target: pass.feed.solutes
                            onSolutesChanged: feedSoluteText.value= pass.feed.solutes.mgl(index);
                        }
                    }
                    ROResultText {
                        id: permeateSoluteText
                        value: pass.permeate.solutes.mgl(index);
                        type: "P"
                        Connections {
                            target: pass.permeate.solutes
                            onSolutesChanged: permeateSoluteText.value= pass.permeate.solutes.mgl(index);
                        }
                    }
                    ROResultText {
                        id: concentrateSoluteText
                        value: pass.concentrate.solutes.mgl(index);
                        type: "C"
                        Connections {
                            target: pass.concentrate.solutes
                            onSolutesChanged: concentrateSoluteText.value= pass.concentrate.solutes.mgl(index);
                        }
                    }
                    //                                    }
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

        ROResultText { value: pass.feed.solutes.lsi; type: "F"; }
        ROResultText { value: pass.permeate.solutes.lsi; type: "P"; }
        ROResultText { value: pass.concentrate.solutes.lsi; type: "C"; }

        ROResultText { value: pass.feed.solutes.sdsi; type: "F"; }
        ROResultText { value: pass.permeate.solutes.sdsi; type: "P"; }
        ROResultText { value: pass.concentrate.solutes.sdsi; type: "C"; }

        ROResultText { value: pass.feed.solutes.ionicStrength; type: "F"; }
        ROResultText { value: pass.permeate.solutes.ionicStrength; type: "P"; }
        ROResultText { value: pass.concentrate.solutes.ionicStrength; type: "C"; }


        Repeater {
            model: ROSolutes.TotalCompounds
            delegate: Column {
                visible: showFieldNum >0 && (sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index))
                ROResultText {
                    id: feedSaturation
                    value: pass.feed.solutes.saturation(index) * 100.0;
                    type: "F";
                    Connections {
                        target: pass.feed.solutes
                        onSolutesChanged: feedSaturation.value = pass.feed.solutes.saturation(index) * 100.0;
                    }
                }

                ROResultText {
                    id: permeateSaturation
                    value: pass.permeate.solutes.saturation(index) * 100.0;
                    type: "P";
                    Connections {
                        target: pass.permeate.solutes
                        onSolutesChanged: permeateSaturation.value = pass.permeate.solutes.saturation(index) * 100.0;
                    }
                }

                ROResultText {
                    id: concentrateSaturation
                    value: pass.concentrate.solutes.saturation(index) * 100.0;
                    type: "C";
                    Connections {
                        target: pass.concentrate.solutes
                        onSolutesChanged: concentrateSaturation.value = pass.concentrate.solutes.saturation(index) * 100.0;
                    }
                }
            }
        }
    }
}
