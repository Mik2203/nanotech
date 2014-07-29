// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets
import "MenuPanel"

Rectangle {
    color: "transparent" // TODO otherwise doesnot update bg

    property real rowHeaderWidth: 150
    property real imageColWidth: 50
    property real m3CostColWidth: 70
    property real cellWidth: 100
    property real cellHeight: 20
    property real colSpacing: 10
    property real rowSpacing: 5

    PrintPanel {
        id: printButtons
        z: 2 // UNCOMMENT TO BE ON TOP
        visible: sysSS.solved
        anchors.right:  parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        onPrint_: app.printer.printCosts(device)
    }

    ROWidgets.ScrollArea {
        anchors.fill: parent

        frame: false


        Row {
            Column {
                Rectangle {
                    width: imageColWidth
                    height: (cellHeight + rowSpacing) * 4 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Image {
                        source: "../images/results-done.png";
                        anchors.centerIn: parent
                        width: 30
                        height: 30
                        smooth: true
                    }
                }

                Rectangle {
                    width: imageColWidth
                    height: (cellHeight + rowSpacing) * 5 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Image {
                        source: "../images/system.png";
                        anchors.centerIn: parent
                        width: 30
                        height: 30
                        smooth: true
                    }
                }

                Rectangle {
                    width: imageColWidth
                    height: (cellHeight + rowSpacing) * 8 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Image {
                        source: "../images/acid.png";
                        anchors.centerIn: parent
                        width: 30
                        height: 30
                        smooth: true
                    }
                }

                Rectangle {
                    width: imageColWidth
                    height: (cellHeight + rowSpacing) * 3 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Image {
                        source: "../images/feedwater.png";
                        anchors.centerIn: parent
                        width: 30
                        height: 30
                        smooth: true
                    }
                }

                Rectangle {
                    width: imageColWidth
                    height: (cellHeight + rowSpacing) * 5 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Image {
                        source: "../images/costs.png";
                        anchors.centerIn: parent
                        width: 30
                        height: 30
                        smooth: true
                    }
                }

            }

            Column {
                Rectangle {
                    width: m3CostColWidth
                    height: (cellHeight + rowSpacing) * 4 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Column {
                        anchors.centerIn: parent
                        Text {
                            text: case_.costs.totalWaterCostsYear.toFixed(app.visibleDecimals);
                            font.bold: true
                            font.pointSize: 12
                            anchors.horizontalCenter: parent.horizontalCenter;
                        }
                        Text {
                            text: app.translator.emptyString + qsTr("rub/m³");
                            anchors.horizontalCenter: parent.horizontalCenter;
                        }
                    }


                }

                Rectangle {
                    width: m3CostColWidth
                    height: (cellHeight + rowSpacing) * 5 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Column {
                        anchors.centerIn: parent
                        Text {
                            text: case_.costs.totalWaterCostsYearSystem.toFixed(app.visibleDecimals);
                            anchors.horizontalCenter: parent.horizontalCenter;
                            font.pointSize: 12
                        }
                        Text { text: app.translator.emptyString + qsTr("rub/m³"); anchors.horizontalCenter: parent.horizontalCenter; }
                    }
                }

                Rectangle {
                    width: m3CostColWidth
                    height: (cellHeight + rowSpacing) * 8 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Column {
                        anchors.centerIn: parent
                        Text {
                            text: case_.costs.totalWaterCostsYearScaling.toFixed(app.visibleDecimals);
                            anchors.horizontalCenter: parent.horizontalCenter;
                            font.pointSize: 12
                        }
                        Text { text: app.translator.emptyString + qsTr("rub/m³"); anchors.horizontalCenter: parent.horizontalCenter; }
                    }
                }

                Rectangle {
                    width: m3CostColWidth
                    height: (cellHeight + rowSpacing) * 3 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Column {
                        anchors.centerIn: parent
                        Text { text: case_.costs.totalWaterCostsYearWater.toFixed(app.visibleDecimals); anchors.horizontalCenter: parent.horizontalCenter; font.pointSize: 12 }
                        Text { text: app.translator.emptyString + qsTr("rub/m³"); anchors.horizontalCenter: parent.horizontalCenter; }
                    }
                }

                Rectangle {
                    width: m3CostColWidth
                    height: (cellHeight + rowSpacing) * 5 + cellHeight + 10
                    border.width: 1
                    border.color: "lightgrey"

                    Column {
                        anchors.centerIn: parent
                        Text { text: case_.costs.totalWaterCostsYearMaintenance.toFixed(app.visibleDecimals); anchors.horizontalCenter: parent.horizontalCenter; font.pointSize: 12 }
                        Text { text: app.translator.emptyString + qsTr("rub/m³"); anchors.horizontalCenter: parent.horizontalCenter; }
                    }
                }

            }

            Column {
                Row {
                    id: totalCosts

                    Rectangle {
                        width: totalDataCol.width + totalDataCol.anchors.margins * 2
                        height: totalDataCol.height + totalDataCol.anchors.margins * 2
                        border.width: 1
                        border.color: "lightgrey"

                        Column {
                            id: totalDataCol
                            spacing: rowSpacing
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 5

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Year permeate (m³)"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.permeateYearAmount; }
                                Item { width: (cellWidth + colSpacing) * 4 + cellWidth; height: cellHeight }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("System cost (rub)"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsEditableCell { value: case_.costs.systemCost; onInputChanged: case_.costs.systemCost = changedValue; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("System set cost (rub)"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.systemSetCostYear }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("System with set cost (rub)"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.systemWithYearSetCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Total year cost (rub)"); font.bold: true; anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.totalCostsYear; font.bold: true; }
                            }
                        }
                    }
                }


                Row {
                    id: sysCosts

                    Rectangle {
                        width: sysDataCol.width + sysDataCol.anchors.margins * 2
                        height: sysDataCol.height + sysDataCol.anchors.margins * 2
                        border.width: 1
                        border.color: "lightgrey"

                        Column {
                            id: sysDataCol
                            spacing: rowSpacing
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 5

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Items by element") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Items by system") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Change times a year") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Items of a year") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Item cost (rub)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Year costs (rub)") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Microfiltes requirement"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsEditableCell { value: case_.costs.microfiltersMembraneCount; onInputChanged: case_.costs.microfiltersMembraneCount = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.microfiltersSystemCount; }
                                ROCostsEditableCell { value: case_.costs.microfiltersYearChangeFrequency; onInputChanged: case_.costs.microfiltersYearChangeFrequency = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.microfiltersYearCount }
                                ROCostsEditableCell { value: case_.costs.microfilterCost; onInputChanged: case_.costs.microfilterCost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.microfiltersYearCost }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Membrane elements requirement"); anchors.verticalCenter: parent.verticalCenter }
                                Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: sys.elementsCount; }
                                ROCostsEditableCell { value: case_.costs.membranesYearChangeFrequency; onInputChanged: case_.costs.membranesYearChangeFrequency = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.membranesYearCount }
                                ROCostsEditableCell { value: case_.costs.membraneCost; onInputChanged: case_.costs.membraneCost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.membranesYearCost }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Energy by 1m³ (kW/h)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Energy by system (kW/h)") }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Energy of a year"); colSpan: 2 }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Energy cost (rub/(kW/h))") }
                                //ROCostsColHeader { width: cellWidth; text: app.translator.emptyString + qsTr("year costs") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Energy requirement"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: sys.specificEnergy }
                                ROCostsReadonlyCell { value: sys.power }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.electricEnergyYearAmount; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.electricEnergyCost; onInputChanged: case_.costs.electricEnergyCost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.electricEnergyYearCost }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Total"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.totalCostsYearSystem; colSpan: 6 }
                            }

                        }
                    }
                }

                Row {
                    id: scalingCosts

                    Rectangle {
                        width: scalingDataCol.width + scalingDataCol.anchors.margins * 2
                        height: scalingDataCol.height + scalingDataCol.anchors.margins * 2
                        border.width: 1
                        border.color: "lightgrey"

                        Column {
                            id: scalingDataCol
                            spacing: rowSpacing
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 5

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Sink (m³)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Sink (kg)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Sink times a year") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Mass of a year (kg)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Item cost (rub/kg)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Year costs (rub)") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Citric acid requirement"); anchors.verticalCenter: parent.verticalCenter }
                                //Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.washingSolutionVolume; rowSpan: 4 }
                                ROCostsReadonlyCell { value: case_.costs.citricAcidAmount; }
                                ROCostsEditableCell { value: case_.costs.washingFrequency; onInputChanged: case_.costs.washingFrequency = changedValue; rowSpan: 4 }
                                ROCostsReadonlyCell { value: case_.costs.citricAcidYearAmount }
                                ROCostsEditableCell { value: case_.costs.citricAcidCost; onInputChanged: case_.costs.citricAcidCost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.citricAcidYearCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("H2SO4 requirement"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.H2SO4Amount; colSpan: 2 }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.H2SO4YearAmount; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.H2SO4Cost; onInputChanged: case_.costs.H2SO4Cost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.H2SO4YearCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Trilon B requirement"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.trilonBAmount; colSpan: 2 }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.trilonBYearAmount; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.trilonBCost; onInputChanged: case_.costs.trilonBCost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.trilonBYearCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Alkali requirement"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.alkaliAmount; colSpan: 2 }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.alkaliYearAmount; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.alkaliCost; onInputChanged: case_.costs.alkaliCost = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.alkaliYearCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Rate (mg/l)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Rate by sys (kg/h)") }
                                //Item { width: cellWidth; height: cellHeight }
                                //ROCostsColHeader { width: cellWidth; text: app.translator.emptyString + qsTr("mass of a year") }
                                //ROCostsColHeader { width: cellWidth; text: app.translator.emptyString + qsTr("item cost") }
                                //ROCostsColHeader { width: cellWidth; text: app.translator.emptyString + qsTr("year costs") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Antiscalant requirement"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsEditableCell { value: case_.costs.antiscalantAmount; onInputChanged: case_.costs.antiscalantAmount = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.antiscalantSystemAmount; }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.antiscalantYearAmount; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.antiscalantCost; onInputChanged: case_.costs.antiscalantCost = changedValue;  }
                                ROCostsReadonlyCell { value: case_.costs.antiscalantYearCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Acid-antiscalant requirement"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsEditableCell { value: case_.costs.acidAntiscalantAmount; onInputChanged: case_.costs.acidAntiscalantAmount = changedValue; }
                                ROCostsReadonlyCell { value: case_.costs.acidAntiscalantSystemAmount; }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.acidAntiscalantYearAmount; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.acidAntiscalantCost; onInputChanged: case_.costs.acidAntiscalantCost = changedValue;  }
                                ROCostsReadonlyCell { value: case_.costs.acidAntiscalantYearCost; }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Total"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.totalCostsYearScaling; colSpan: 6 }
                            }
                        }
                    }
                }

                Row {
                    id: waterCosts

                    Rectangle {
                        width: waterDataCol.width + waterDataCol.anchors.margins * 2
                        height: waterDataCol.height + waterDataCol.anchors.margins * 2
                        border.width: 1
                        border.color: "lightgrey"

                        Column {
                            id: waterDataCol
                            spacing: rowSpacing
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 5

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Rate (m³/h)"); colSpan: 3 }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Volume by year (m³)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Cost (rub/m³)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Year costs") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Raw water"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: sys.feed.rate; colSpan: 3 }
                                ROCostsReadonlyCell { value: case_.costs.rawWaterYearAmount }
                                ROCostsEditableCell { value: case_.costs.rawWaterCost; onInputChanged: case_.costs.rawWaterCost = changedValue;  }
                                ROCostsReadonlyCell { value: case_.costs.rawWaterYearCost }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Concentrate drop"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: sys.concentrate.rate; colSpan: 3 }
                                ROCostsReadonlyCell { value: case_.costs.concentrateDropYearAmount }
                                ROCostsEditableCell { value: case_.costs.concentrateDropCost;  onInputChanged: case_.costs.concentrateDropCost = changedValue;  }
                                ROCostsReadonlyCell { value: case_.costs.concentrateDropYearCost }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Total"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.totalCostsYearWater; colSpan: 6 }
                            }
                        }
                    }
                }

                Row {
                    id: staffCosts

                    Rectangle {
                        width: staffDataCol.width + staffDataCol.anchors.margins * 2
                        height: staffDataCol.height + staffDataCol.anchors.margins * 2
                        border.width: 1
                        border.color: "lightgrey"

                        Column {
                            id: staffDataCol
                            spacing: rowSpacing
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 5

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Salary (rub/month)"); colSpan: 2 }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Staff count") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Year salary (rub/month)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Overhead rate (%)") }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("Year costs (rub)") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Staff salary"); anchors.verticalCenter: parent.verticalCenter }
                                //Item { width: cellWidth; height: cellHeight }
                                ROCostsEditableCell { value: case_.costs.staffSalary; onInputChanged: case_.costs.staffSalary = changedValue; colSpan: 2 }
                                ROCostsEditableCell { value: case_.costs.staffCount; onInputChanged: case_.costs.staffCount = changedValue; rowSpan: 2 }
                                ROCostsReadonlyCell { value: case_.costs.staffYearSalary; }
                                ROCostsEditableCell { value: case_.costs.overheadRate * 100.0; onInputChanged: case_.costs.overheadRate = changedValue / 100.0; rowSpan: 2 }
                                ROCostsReadonlyCell { value: case_.costs.staffYearCost }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Accruals"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsEditableCell { value: case_.costs.accruals; onInputChanged: case_.costs.accruals = changedValue; colSpan: 2 }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.accrualsYear; colSpan: 2 }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsReadonlyCell { value: case_.costs.accrualsYearCost; colSpan: 2 }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { } // skip cell
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsColHeader { text: app.translator.emptyString + qsTr("System lifetime (years)"); colSpan: 5 }
                                //ROCostsColHeader { width: cellWidth; text: app.translator.emptyString + qsTr("%") }
                                //ROCostsColHeader { width: cellWidth; text: app.translator.emptyString + qsTr("year costs") }
                            }

                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Deductions"); anchors.verticalCenter: parent.verticalCenter }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                //                            Item { width: cellWidth; height: cellHeight }
                                ROCostsEditableCell { value: case_.costs.systemLifetime; onInputChanged: case_.costs.systemLifetime = changedValue; colSpan: 5 }
                                ROCostsReadonlyCell { value: case_.costs.deductionsYearCost }
                            }


                            Row {
                                spacing: colSpacing
                                ROCostsRowHeader { text: app.translator.emptyString + qsTr("Total"); anchors.verticalCenter: parent.verticalCenter }
                                ROCostsReadonlyCell { value: case_.costs.totalCostsYearMaintenance; colSpan: 6 }
                            }
                        }
                    }
                }

            }

        }

    }
}
