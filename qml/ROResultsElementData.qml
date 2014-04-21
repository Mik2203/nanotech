// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import ROElement 1.0
import ROSolutes 1.0
import ROUnits 1.0

Column {
    property ROElement element: stage.element(index)


    width: colWidthData //visible ? : 01
    Behavior on width { NumberAnimation { duration: 100 } }

    ROResultText { value: element.activeArea }
    ROResultText { value: app.units.convertFluxUnits(element.averageFlux, ROUnits.DEFAULT_FLUX_UNITS, app.units.fluxUnits); }
    ROResultText { value: element.recovery * 100.0; }
    ROResultText { value: app.units.convertFlowUnits(element.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "F"; }
    ROResultText { value: app.units.convertFlowUnits(element.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "P"; }
    ROResultText { value: app.units.convertFlowUnits(element.concentrate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits); type: "C"; }

    ROResultText { value: app.units.convertPressureUnits(element.feed.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
    ROResultText { value: app.units.convertPressureUnits(element.permeate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
    ROResultText { value: app.units.convertPressureUnits(element.concentrate.pressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }
    ROResultText { value: app.units.convertPressureUnits(element.feed.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "F"; }
    ROResultText { value: app.units.convertPressureUnits(element.permeate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "P"; }
    ROResultText { value: app.units.convertPressureUnits(element.concentrate.osmoticPressure, ROUnits.DEFAULT_PRESSURE_UNITS, app.units.pressureUnits); type: "C"; }



    Item { // SOLUTES HEADER SPACER
        //width: 30
        width: colWidthData
        height: rowHeight + solutesHeaderOffset
        //visible: sys.feed.solutes.isFilled
    }

    Column { // SOLUTES
        ROResultText { value: element.feed.solutes.pH; type: "F"; }
        ROResultText { value: element.permeate.solutes.pH; type: "P"; }
        ROResultText { value: element.concentrate.solutes.pH; type: "C"; }
        ROResultText { value: element.feed.solutes.totalValueMgl; type: "F"; }
        ROResultText { value: element.permeate.solutes.totalValueMgl; type: "P"; }
        ROResultText { value: element.concentrate.solutes.totalValueMgl; type: "C"; }

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
                    value: element.feed.solutes.mgl(index);
                    type: "F"
                    Connections {
                        target: element.feed.solutes
                        onSolutesChanged: feedSoluteText.value= element.feed.solutes.mgl(index);
                    }
                }
                ROResultText {
                    id: permeateSoluteText
                    value: element.permeate.solutes.mgl(index);
                    type: "P"
                    Connections {
                        target: element.permeate.solutes
                        onSolutesChanged: permeateSoluteText.value= element.permeate.solutes.mgl(index);
                    }
                }
                ROResultText {
                    id: concentrateSoluteText
                    value: element.concentrate.solutes.mgl(index);
                    type: "C"
                    Connections {
                        target: element.concentrate.solutes
                        onSolutesChanged: concentrateSoluteText.value= element.concentrate.solutes.mgl(index);
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

    ROResultText { value: element.feed.solutes.lsi; type: "F"; }
    ROResultText { value: element.permeate.solutes.lsi; type: "P"; }
    ROResultText { value: element.concentrate.solutes.lsi; type: "C"; }

    ROResultText { value: element.feed.solutes.sdsi; type: "F"; }
    ROResultText { value: element.permeate.solutes.sdsi; type: "P"; }
    ROResultText { value: element.concentrate.solutes.sdsi; type: "C"; }

    ROResultText { value: element.feed.solutes.ionicStrength; type: "F"; }
    ROResultText { value: element.permeate.solutes.ionicStrength; type: "P"; }
    ROResultText { value: element.concentrate.solutes.ionicStrength; type: "C"; }


    Repeater {
        model: ROSolutes.TotalCompounds
        delegate: Column {
            visible: showFieldNum >0 && (sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index))
            ROResultText {
                id: feedSaturation
                value: element.feed.solutes.saturation(index) * 100.0;
                type: "F";
                Connections {
                    target: element.feed.solutes
                    onSolutesChanged: feedSaturation.value = element.feed.solutes.saturation(index) * 100.0;
                }
            }

            ROResultText {
                id: permeateSaturation
                value: element.permeate.solutes.saturation(index) * 100.0;
                type: "P";
                Connections {
                    target: element.permeate.solutes
                    onSolutesChanged: permeateSaturation.value = element.permeate.solutes.saturation(index) * 100.0;
                }
            }

            ROResultText {
                id: concentrateSaturation
                value: element.concentrate.solutes.saturation(index) * 100.0;
                type: "C";
                Connections {
                    target: element.concentrate.solutes
                    onSolutesChanged: concentrateSaturation.value = element.concentrate.solutes.saturation(index) * 100.0;
                }
            }
        }
    }
}
