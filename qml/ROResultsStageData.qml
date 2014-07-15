// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


import ROStage 1.0
import ROSolutes 1.0
import ROUnits 1.0


Row {
    property ROStage stage: pass.stage(index)

    Repeater {
        model: stage.elementsPerVesselCount

        delegate: Loader {
            Component {
                id: elementComponent
                ROResultsElementData{}
            }

            sourceComponent: expandFlags[stage] & expandFlags[pass] & expandFlags[sys] ? elementComponent : nullWidthStub
        }
    }


    Column { // STAGE DATA
//        Behavior on width { NumberAnimation { duration: 100 } }

        ROResultText { value: stage.activeArea }
        ROResultText { value: app.units.convertFluxUnits(stage.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits); }
        ROResultText { value: stage.recovery * 100.0; }
        ROResultText { value: app.units.convertFlowUnits(stage.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "F"; }
        ROResultText { value: app.units.convertFlowUnits(stage.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "P"; }
        ROResultText { value: app.units.convertFlowUnits(stage.concentrate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "C"; }
        ROResultText { value: app.units.convertPressureUnits(stage.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
        ROResultText { value: app.units.convertPressureUnits(stage.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
        ROResultText { value: app.units.convertPressureUnits(stage.concentrate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }
        ROResultText { value: app.units.convertPressureUnits(stage.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
        ROResultText { value: app.units.convertPressureUnits(stage.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
        ROResultText { value: app.units.convertPressureUnits(stage.concentrate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }


        Item { // SOLUTES HEADER SPACER
            //width: 30
            width: colWidthData
            height: rowHeight + solutesHeaderOffset
        }

        Column { // SOLUTES
            ROResultText { value: stage.feed.pH; type: "F"; }
            ROResultText { value: stage.permeate.pH; type: "P"; }
            ROResultText { value: stage.concentrate.pH; type: "C"; }

            ROResultText { value: stage.feed.solutes.totalValueMgl; type: "F"; }
            ROResultText { value: stage.permeate.solutes.totalValueMgl; type: "P"; }
            ROResultText { value: stage.concentrate.solutes.totalValueMgl; type: "C"; }

            Component {
                id: solutesDelegate

                Column {
                    id: soluteCell
                    visible: (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))

                    Connections {
                        target: sys.feed.solutes
                        onSolutesChanged: soluteCell.visible= (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))
                    }
                    ROResultText {
                        id: feedSoluteText
                        value: stage.feed.solutes.mgl(index);
                        type: "F"
                        Connections {
                            target: stage.feed.solutes
                            onSolutesChanged: feedSoluteText.value= stage.feed.solutes.mgl(index);
                        }
                    }
                    ROResultText {
                        id: permeateSoluteText
                        value: stage.permeate.solutes.mgl(index);
                        type: "P"
                        Connections {
                            target: stage.permeate.solutes
                            onSolutesChanged: permeateSoluteText.value= stage.permeate.solutes.mgl(index);
                        }
                    }
                    ROResultText {
                        id: concentrateSoluteText
                        value: stage.concentrate.solutes.mgl(index);
                        type: "C"
                        Connections {
                            target: stage.concentrate.solutes
                            onSolutesChanged: concentrateSoluteText.value= stage.concentrate.solutes.mgl(index);
                        }
                    }
                    //}
                }

            }

            Repeater {
                model: ROSolutes.TotalIons

                delegate: solutesDelegate
            }
        }

        Item { // SCALING HEADER SPACER
            //width: 30
            width: colWidthData
            height: rowHeight + solutesHeaderOffset
        }

        ROResultText { value: stage.feed.solutes.lsi; type: "F"; }
        ROResultText { value: stage.permeate.solutes.lsi; type: "P"; }
        ROResultText { value: stage.concentrate.solutes.lsi; type: "C"; }

        ROResultText { value: stage.feed.solutes.sdsi; type: "F"; }
        ROResultText { value: stage.permeate.solutes.sdsi; type: "P"; }
        ROResultText { value: stage.concentrate.solutes.sdsi; type: "C"; }

        ROResultText { value: stage.feed.solutes.ionicStrength; type: "F"; }
        ROResultText { value: stage.permeate.solutes.ionicStrength; type: "P"; }
        ROResultText { value: stage.concentrate.solutes.ionicStrength; type: "C"; }

        Repeater {
            model: ROSolutes.TotalCompounds
            delegate: Column {
                visible: showFieldNum >0 && (sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index))
                ROResultText {
                    id: feedSaturation
                    value: stage.feed.solutes.saturation(index) * 100.0;
                    type: "F";
                    Connections {
                        target: stage.feed.solutes
                        onSolutesChanged: feedSaturation.value = stage.feed.solutes.saturation(index) * 100.0;
                    }
                }

                ROResultText {
                    id: permeateSaturation
                    value: stage.permeate.solutes.saturation(index) * 100.0;
                    type: "P";
                    Connections {
                        target: stage.permeate.solutes
                        onSolutesChanged: permeateSaturation.value = stage.permeate.solutes.saturation(index) * 100.0;
                    }
                }

                ROResultText {
                    id: concentrateSaturation
                    value: stage.concentrate.solutes.saturation(index) * 100.0;
                    type: "C";
                    Connections {
                        target: stage.concentrate.solutes
                        onSolutesChanged: concentrateSaturation.value = stage.concentrate.solutes.saturation(index) * 100.0;
                    }
                }
            }
        }

    }
}
