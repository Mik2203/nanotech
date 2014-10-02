// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "widgets" as ROWidgets

import ROFlow 1.0
import ROSolutes 1.0
import ROScalingElement 1.0


// TODO - разделить потоки на столбцы, а не по строкам

Column {
    id: column_panel
    property bool showAdjustedFeed: true // TODO - после разделения на столбцы - есть смысл показывать скорректированный поток только при изменениях

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            id: feedHeader
            text: app.translator.emptyString + qsTr("Feed")
            anchors.right: adjFeedHeader.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignLeft
            height: parent.height-3
            width: 50
        }

        Text {
            id: adjFeedHeader
            text: app.translator.emptyString + qsTr("Adj. Feed")
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignLeft
            height: parent.height-3
            width: 50

        }

    }


    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("pH:")
        }

        ROWidgets.DoubleInput {
            id: phFeedLabel
            anchors.right: phAdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.pH
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: phAdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.pH
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }

    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("LSI:")
        }

        ROWidgets.DoubleInput {
            id: lsiFeedLabel
            anchors.right: lsiAdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.lsi
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: lsiAdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.lsi
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }

    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("S&DSI:")
        }

        ROWidgets.DoubleInput {
            id: sdsiFeedLabel
            anchors.right: sdsiAdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.sdsi
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: sdsiAdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.sdsi
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }

    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("TDS (mg/l):")
        }

        ROWidgets.DoubleInput {
            id: tdsFeedLabel
            anchors.right: tdsAdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.totalValueMgl
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: tdsAdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.totalValueMgl
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }

    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("Ionic Strength:")
        }

        ROWidgets.DoubleInput {
            id: icFeedLabel
            anchors.right: icAdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.ionicStrength
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: icAdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.ionicStrength
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("Ionic Strength2:")
        }

        ROWidgets.DoubleInput {
            id: ic2FeedLabel
            anchors.right: ic2AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.ionicStrength2
            editable: false
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: ic2AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.ionicStrength2
            editable: false
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("HCO3 (mg/l):")
        }

        ROWidgets.DoubleInput {
            id: hco3FeedLabel
            anchors.right: hco3AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.mgl(ROSolutes.HCO3)
            editable: false
            Connections {
                target: sys.feed.solutes
                onSolutesChanged: hco3FeedLabel.value = sys.feed.solutes.mgl(ROSolutes.HCO3)
            }

            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: hco3AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.mgl(ROSolutes.HCO3)
            editable: false
            Connections {
                target: sys.adjustedFeed.solutes
                onSolutesChanged: hco3AdjFeedLabel.value = sys.adjustedFeed.solutes.mgl(ROSolutes.HCO3)
            }
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("CO2 (mg/l): ")
        }

        ROWidgets.DoubleInput {
            id: co2FeedLabel
            anchors.right: co2AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.mgl(ROSolutes.CO2)
            editable: false
            Connections {
                target: sys.feed.solutes
                onSolutesChanged: co2FeedLabel.value = sys.feed.solutes.mgl(ROSolutes.CO2)
            }
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: co2AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.mgl(ROSolutes.CO2)
            editable: false
            Connections {
                target: sys.adjustedFeed.solutes
                onSolutesChanged: co2AdjFeedLabel.value = sys.adjustedFeed.solutes.mgl(ROSolutes.CO2)
            }
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: app.translator.emptyString + qsTr("CO3 (mg/l): ")
        }

        ROWidgets.DoubleInput {
            id: co3FeedLabel
            anchors.right: co3AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.mgl(ROSolutes.CO3)
            editable: false
            Connections {
                target: sys.feed.solutes
                onSolutesChanged: co3FeedLabel.value = sys.feed.solutes.mgl(ROSolutes.CO3)
            }
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: co3AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.mgl(ROSolutes.CO3)
            editable: false
            Connections {
                target: sys.adjustedFeed.solutes
                onSolutesChanged: co3AdjFeedLabel.value = sys.adjustedFeed.solutes.mgl(ROSolutes.CO3)
            }
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }

    }


    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "CaSO4 (%):"
        }

        ROWidgets.DoubleInput {
            id: caso4FeedLabel
            anchors.right: caso4AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.saturation(ROSolutes.CaSO4) * 100.0
            editable: false

            Connections {
                target: sys.feed
                onSolutesChanged: caso4FeedLabel.value = sys.feed.solutes.saturation(ROSolutes.CaSO4) * 100.0
            }

            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }


        ROWidgets.DoubleInput {
            id: caso4AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.saturation(ROSolutes.CaSO4) * 100.0
            editable: false

            Connections {
                target: sys.adjustedFeed
                onSolutesChanged: caso4AdjFeedLabel.value = sys.adjustedFeed.solutes.saturation(ROSolutes.CaSO4) * 100.0
            }
            //                KeyNavigation.backtab: passRecoveryInput
            //                KeyNavigation.tab: passFlowFactorInput
        }

    }


    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "BaSO4 (%):"
        }

        ROWidgets.DoubleInput {
            id: baso4FeedLabel
            anchors.right: baso4AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.saturation(ROSolutes.BaSO4, sys.feed.temperature) * 100.0
            editable: false

            Connections {
                target: sys.feed
                onSolutesChanged: baso4FeedLabel.value = sys.feed.solutes.saturation(ROSolutes.BaSO4) * 100.0
            }
        }


        ROWidgets.DoubleInput {
            id: baso4AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.saturation(ROSolutes.BaSO4) * 100.0
            editable: false

            Connections {
                target: sys.adjustedFeed
                onSolutesChanged: baso4AdjFeedLabel.value = sys.adjustedFeed.solutes.saturation(ROSolutes.BaSO4) * 100.0
            }
        }

    }


    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "SrSO4 (%):"
        }

        ROWidgets.DoubleInput {
            id: srso4FeedLabel
            anchors.right: srso4AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.saturation(ROSolutes.SrSO4, sys.feed.temperature) * 100.0
            editable: false

            Connections {
                target: sys.feed
                onSolutesChanged: srso4FeedLabel.value = sys.feed.solutes.saturation(ROSolutes.SrSO4) * 100.0
            }
        }


        ROWidgets.DoubleInput {
            id: srso4AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.saturation(ROSolutes.SrSO4) * 100.0
            editable: false

            Connections {
                target: sys.adjustedFeed
                onSolutesChanged: srso4AdjFeedLabel.value = sys.adjustedFeed.solutes.saturation(ROSolutes.SrSO4) * 100.0
            }
        }

    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "CaF4 (%):"
        }

        ROWidgets.DoubleInput {
            id: caf4FeedLabel
            anchors.right: caf4AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.saturation(ROSolutes.CaF2, sys.feed.temperature) * 100.0
            editable: false

            Connections {
                target: sys.feed
                onSolutesChanged: caf4FeedLabel.value = sys.feed.solutes.saturation(ROSolutes.CaF2) * 100.0
            }
        }


        ROWidgets.DoubleInput {
            id: caf4AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.saturation(ROSolutes.CaF2) * 100.0
            editable: false

            Connections {
                target: sys.adjustedFeed
                onSolutesChanged: caf4AdjFeedLabel.value = sys.adjustedFeed.solutes.saturation(ROSolutes.CaF2) * 100.0
            }
        }

    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "SiO2 (%):"
        }

        ROWidgets.DoubleInput {
            id: sio2FeedLabel
            anchors.right: sio2AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.saturation(ROSolutes.SiO2_) * 100.0
            editable: false

            Connections {
                target: sys.feed
                onSolutesChanged: sio2FeedLabel.value = sys.feed.solutes.saturation(ROSolutes.SiO2_) * 100.0
            }
        }


        ROWidgets.DoubleInput {
            id: sio2AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.saturation(ROSolutes.SiO2_) * 100.0
            editable: false

            Connections {
                target: sys.adjustedFeed
                onSolutesChanged: sio2AdjFeedLabel.value = sys.adjustedFeed.solutes.saturation(ROSolutes.SiO2_) * 100.0
            }
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        Text {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "Mg(OH)2 (%):"
        }

        ROWidgets.DoubleInput {
            id: mgoh2FeedLabel
            anchors.right: mgoh2AdjFeedLabel.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.feed.solutes.saturation(ROSolutes.MgOH2) * 100.0
            editable: false

            Connections {
                target: sys.feed
                onSolutesChanged: mgoh2FeedLabel.value = sys.feed.solutes.saturation(ROSolutes.MgOH2) * 100.0
            }
        }


        ROWidgets.DoubleInput {
            id: mgoh2AdjFeedLabel
            anchors.right: parent.right
            //anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignRight
            height: parent.height-3
            width: 50
            value: sys.adjustedFeed.solutes.saturation(ROSolutes.MgOH2) * 100.0
            editable: false

            Connections {
                target: sys.adjustedFeed
                onSolutesChanged: mgoh2AdjFeedLabel.value = sys.adjustedFeed.solutes.saturation(ROSolutes.MgOH2) * 100.0
            }
        }
    }
}
