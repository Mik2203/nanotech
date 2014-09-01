import QtQuick 1.0
import ROPass 1.0
import ROPassController 1.0
import ROPassParamSetController 1.0
import ROStage 1.0
import ROFlow 1.0
import ROUnits 1.0
import ROUnitsText 1.0
import "widgets" as ROWidgets


Item {
    id: panel
    property ROPass pass
    property int passIndex: sys.passIndex(pass)
    property ROPassController passC: sysC.passC(pass)
    height: column_panel.height
    onPassChanged: selectedStage = pass.firstStage

    function __passHasIncomingRecycles() {
        for (var pi=passIndex+1; pi<sys.passCount; ++pi) {
            if (sys.pass(pi).hasRecycle(passIndex))
                return true;
        }
        return false;
    }
    function __passIncomingRecyclesRate() {
        var rate = 0.0;
        for (var pi=passIndex+1; pi<sys.passCount; ++pi) {
            if (sys.pass(pi).hasRecycle(passIndex))
                rate += sys.pass(pi).recycle(passIndex)
        }
        return rate;
    }
    function __updateRowsVisibility() {
        originalFeedRow.visible = __passHasIncomingRecycles()
        if (originalFeedRow.visible)
            originalFeedRow.originalFeed = pass.feed.rate - __passIncomingRecyclesRate()
        stageFeedRow.visible = __passHasIncomingRecycles() || pass.hasSelfRecycle || pass.hasBlendPermeate
    }
    onPassIndexChanged: __updateRowsVisibility()
    Connections {
        target: pass
        onHasSelfRecycleChanged: __updateRowsVisibility()
        onHasBlendPermeateChanged: __updateRowsVisibility()
    }
    Repeater {
        model: sys.passCount
        Connections {
            target: sys.pass(index)
            onRecycleChanged: __updateRowsVisibility()
        }
    }


    Column {
        id: column_panel
        width: panel.width

        Item { // PASS PERMEATE FLOW EDITOR ROW
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Permeate flow (P%1):").arg(passIndex+1)
            }

            ROWidgets.CheckBox {
                id: passPermeateFlowSetController
                anchors.right: passPermeateFlowInput.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                checked: passC.paramSetC.permeateSetState == ROPassParamSetController.ParamSetExplicit
                onToggle: {
                    if (passC.paramSetC.permeateSetState == ROPassParamSetController.ParamSetExplicit)
                        passC.paramSetC.permeateSetState = ROPassParamSetController.ParamSetUndefined
                    else passC.paramSetC.permeateSetState = ROPassParamSetController.ParamSetExplicit
                }
                enabled: (passC.paramSetC.permeateSetState == ROPassParamSetController.ParamSetExplicit ||
                          passC.paramSetC.permeateSetState == ROPassParamSetController.ParamSetUndefined)
            }

            ROWidgets.DoubleInput {
                id: passPermeateFlowInput
                anchors.right: passPermeateFlowUnits.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                height: parent.height-3
                width: 50
                value: app.units.convertFlowUnits(pass.permeate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits)
                onInputChanged: {
                    pass.permeate.rate = app.units.convertFlowUnits(changedValue, app.units.flowUnits, ROUnits.DEFAULT_FLOW_UNITS)
                    passC.paramSetC.permeateSetState = ROPassParamSetController.ParamSetExplicit
                }
                editable: passPermeateFlowSetController.enabled
                KeyNavigation.backtab: passRecoveryInput
//                KeyNavigation.tab: passFlowFactorInput
            }

            Text {
                id: passPermeateFlowUnits
                anchors.right: parent.right
                anchors.rightMargin: parent.height-3
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                font.italic: true
                horizontalAlignment: Text.AlignLeft
                width: 30
            }


        }



        Item { // PASS RECOVERY EDITOR ROW
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Recovery:")
            }

            ROWidgets.CheckBox {
                id: passRecoverySetController
                anchors.right: passRecoveryInput.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                checked: passC.paramSetC.recoverySetState == ROPassParamSetController.ParamSetExplicit
                onToggle: {
                    if (passC.paramSetC.recoverySetState == ROPassParamSetController.ParamSetExplicit)
                        passC.paramSetC.recoverySetState = ROPassParamSetController.ParamSetUndefined
                    else passC.paramSetC.recoverySetState = ROPassParamSetController.ParamSetExplicit
                }
                enabled: (passC.paramSetC.recoverySetState == ROPassParamSetController.ParamSetExplicit ||
                          passC.paramSetC.recoverySetState == ROPassParamSetController.ParamSetUndefined)
            }

            ROWidgets.DoubleInput {
                id: passRecoveryInput
                anchors.right: passRecoveryUnits.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                height: parent.height-3
                width: 50
                value: pass.recovery * 100.0
                validator: DoubleValidator { bottom: pass.MIN_RECOVERY; top: pass.MAX_RECOVERY }
                onInputChanged: {
                    pass.recovery = changedValue / 100.0
                    passC.paramSetC.recoverySetState = ROPassParamSetController.ParamSetExplicit
                }
                editable: passRecoverySetController.enabled
                KeyNavigation.tab: passPermeateFlowInput
                KeyNavigation.backtab: passFeedFlowInput
            }

            Text {
                id: passRecoveryUnits
                anchors.right: parent.right
                anchors.rightMargin: parent.height-3
                anchors.verticalCenter: parent.verticalCenter
                text: "%"
                font.italic: true
                horizontalAlignment: Text.AlignLeft
                width: 30
            }


            ROWidgets.WarningPopup {
                id: checkRecoveryWarning
                warning: passC.checkRecovery
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }

        }


        Item { // PASS FEED FLOW EDITOR ROW
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + qsTr("Feed flow:")
            }

            ROWidgets.CheckBox {
                id: passFeedFlowSetController
                anchors.right: passFeedFlowInput.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                checked: passC.paramSetC.feedSetState == ROPassParamSetController.ParamSetExplicit
                onToggle: {
                    if (passC.paramSetC.feedSetState == ROPassParamSetController.ParamSetExplicit)
                        passC.paramSetC.feedSetState = ROPassParamSetController.ParamSetUndefined
                    else
                        passC.paramSetC.feedSetState = ROPassParamSetController.ParamSetExplicit
                }
                enabled: (passC.paramSetC.feedSetState == ROPassParamSetController.ParamSetExplicit ||
                          passC.paramSetC.feedSetState == ROPassParamSetController.ParamSetUndefined)
            }

            ROWidgets.DoubleInput {
                id: passFeedFlowInput
                anchors.right: passFeedFlowUnits.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                height: parent.height-3
                width: 50
                value: app.units.convertFlowUnits(pass.feed.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits)
                onInputChanged: {
                    pass.feed.rate = app.units.convertFlowUnits(changedValue, app.units.flowUnits, ROUnits.DEFAULT_FLOW_UNITS)
                    passC.paramSetC.feedSetState = ROPassParamSetController.ParamSetExplicit
                }
                editable: passFeedFlowSetController.enabled
                KeyNavigation.tab: passRecoveryInput
            }

            Text {
                id: passFeedFlowUnits
                anchors.right: parent.right//passFeedFlowRightSpacer.left
                anchors.verticalCenter: parent.verticalCenter
                text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                font.italic: true
                horizontalAlignment: Text.AlignLeft
                width: 30
                anchors.rightMargin: parent.height-3
            }


            ROWidgets.WarningPopup {
                id: flowDataFillWarning
                enabled: passC ? true : false
                warning: passC.fillFlowData
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }



//        Item { // PASS CONCENTRATE ROW
//            anchors.left: parent.left
//            anchors.right: parent.right
//            height: 20

//            Text {
//                anchors.left: parent.left
//                anchors.verticalCenter: parent.verticalCenter
//                text: app.translator.emptyString + qsTr("Concentrate flow (C%1):").arg(passIndex+1)
//            }

//            Text {
//                id: passConcentrateFlowLabel
//                anchors.right: passConcentrateFlowUnits.left
//                anchors.rightMargin: 5
//                anchors.verticalCenter: parent.verticalCenter
//                horizontalAlignment: TextInput.AlignRight
//                width: 50
//                text: app.units.convertFlowUnits(pass.concentrate.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2)
//            }

//            Text {
//                id: passConcentrateFlowUnits
//                anchors.right: parent.right
//                anchors.verticalCenter: parent.verticalCenter
//                text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
//                font.italic: true
//                horizontalAlignment: Text.AlignLeft
//                width: 30
//                anchors.rightMargin: parent.height-3
//            }
//        }


        Item { // PASS FLUX ROW
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.italic: true
                text: app.translator.emptyString + qsTr("Average flux:")
            }

            Text {
                id: passFluxLabel
                anchors.right: parent.right//passFlowFactorUnitsSpacer.left
                anchors.rightMargin: 5 + 30 + parent.height-3
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: TextInput.AlignRight
                width: 50
                font.italic: true
                text: pass.averageFlux.toFixed(2)
            }
        }


        Column {
            anchors.left: parent.left
            anchors.right: parent.right

            Item { // PASS BLEND PERMEATE EDITOR ROW
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                property bool editable: passIndex == sys.passCount-1  // is last pass

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + qsTr("Blend permeate (B%1):").arg(passIndex+1)
                }

                ROWidgets.CheckBox {
                    id: blendPermeateToggler
                    anchors.right: blendPermeateEditor.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: parent.editable

                    checked: pass.hasBlendPermeate
                    onToggle: sys.hasBlendPermeate = state
                }

                ROWidgets.DoubleInput {
                    id: blendPermeateEditor
                    anchors.right: blendPermeateUnits.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignRight
                    height: parent.height-3
                    width: 50
                    editable: parent.editable

                    value: app.units.convertFlowUnits(pass.blendPermeate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits)
                    onInputChanged: {
                        if (!sys.hasBlendPermeate)
                            sys.hasBlendPermeate = true;
                        pass.blendPermeate = app.units.convertFlowUnits(changedValue, app.units.flowUnits, ROUnits.DEFAULT_FLOW_UNITS)
                    }
//                    KeyNavigation.backtab: passFlowFactorInput
                }

                Text {
                    id: blendPermeateUnits
                    anchors.right: parent.right
                    anchors.rightMargin: parent.height-3
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                    font.italic: true
                    horizontalAlignment: Text.AlignLeft
                    width: 30
                }

                Image {
                    id: blendInfo

                    height: 15
                    width: 15

                    source: "../images/warning_info.png"
                    smooth: true
                    visible: !parent.editable
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onHoveredChanged: {
                            if (containsMouse) {
                                mainWindow.autoPopup(popupComponent, blendInfo)
                            } else if (mainWindow.popupComponent == popupComponent) {
                                mainWindow.hidePopup()
                            }
                        }

                        Component {
                            id: popupComponent
                            ROWidgets.BorderText {
                                text: qsTr("Blending is possible only on last pass");
                            }
                        }

                    }
                }

            }

            Column {
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right

                visible: pass.hasBlendPermeate// && pass.blendPermeate > 0.0

//                Item {
//                    anchors.left: parent.left
//                    anchors.right: parent.right
//                    height: 20

//                    Text {
//                        anchors.left: parent.left
//                        anchors.verticalCenter: parent.verticalCenter
//                        text: app.translator.emptyString + qsTr("Feed on stages (SF%1):").arg(passIndex+1)
//                    }

//                    Text {
//                        id: feedOnStageValue
//                        anchors.right: feedOnStageUnits.left
//                        anchors.rightMargin: 5
//                        anchors.verticalCenter: parent.verticalCenter
//                        horizontalAlignment: Text.AlignRight
//                        verticalAlignment: Text.AlignVCenter
//                        height: parent.height-3
//                        width: 50
//                        text: app.units.convertFlowUnits(pass.feed.rate - pass.blendPermeate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2)
//                        // KeyNavigation.backtab: passFlowFactorInput
//                    }

//                    Text {
//                        id: feedOnStageUnits
//                        anchors.right: parent.right
//                        anchors.rightMargin: parent.height-3
//                        anchors.verticalCenter: parent.verticalCenter
//                        text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
//                        font.italic: true
//                        horizontalAlignment: Text.AlignLeft
//                        width: 30
//                    }
//                }

                Item { // PASS BLEND PERMEATE EDITOR ROW
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 20

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        font.italic: true
                        text: app.translator.emptyString + qsTr("Total product (TP%1):").arg(passIndex+1)
                    }

                    Text {
                        id: permeateFromStagesValue
                        anchors.right: permeateFromStagesUnits.left
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        height: parent.height-3
                        width: 50
                        font.italic: true
                        text: app.units.convertFlowUnits(pass.totalProduct.rate, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2)
                        // KeyNavigation.backtab: passFlowFactorInput
                    }

                    Text {
                        id: permeateFromStagesUnits
                        anchors.right: parent.right
                        anchors.rightMargin: parent.height-3
                        anchors.verticalCenter: parent.verticalCenter
                        text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                        font.italic: true
                        horizontalAlignment: Text.AlignLeft
                        width: 30
                    }
                }

            }
        }

        // RECYCLES TEXT
        Item {
            height: 20
            anchors.left: parent.left
            anchors.right: parent.right
            visible: passIndex > 0
            Text {
                text: app.translator.emptyString + qsTr("Recycles:");
                anchors.verticalCenter: parent.verticalCenter
            }
        }



        Column {
            anchors.left: parent.left
            anchors.leftMargin: passIndex > 0 ? 10 : 0
            anchors.right: parent.right
            Item { // PASS SELF RECYCLE FLOW EDITOR ROW
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + qsTr("Self Recycle (SR%1):").arg(passIndex+1)
                }

                ROWidgets.CheckBox {
                    id: selfRecycleToggler
                    anchors.right: selfRecycleEditor.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    checked: pass.hasSelfRecycle
                    onToggle: pass.hasSelfRecycle = state
                }

                ROWidgets.DoubleInput {
                    id: selfRecycleEditor
                    anchors.right: selfRecycleUnits.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignRight
                    height: parent.height-3
                    width: 50
                    value: app.units.convertFlowUnits(pass.selfRecycle, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits)
                    onInputChanged: {
                        if (!pass.hasSelfRecycle)
                            pass.hasSelfRecycle = true;
                        pass.selfRecycle = app.units.convertFlowUnits(changedValue, app.units.flowUnits, ROUnits.DEFAULT_FLOW_UNITS)
                    }
//                    KeyNavigation.backtab: passFlowFactorInput
                }

                Text {
                    id: selfRecycleUnits
                    anchors.right: parent.right
                    anchors.rightMargin: parent.height-3
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                    font.italic: true
                    horizontalAlignment: Text.AlignLeft
                    width: 30
                }
            }


            Component {
                id: recyclesDelegate
                Item { // PASS PERMEATE FLOW EDITOR ROW
                    width: parent.width
                    height: 20

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: app.translator.emptyString + qsTr("Pass %1 (R%2-%1):").arg(index+1).arg(passIndex+1)
                    }

                    ROWidgets.CheckBox {
                        id: recycleToggler
                        anchors.right: recycleEditor.left
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        checked: pass.hasRecycle(index)
                        onToggle: state ? pass.addRecycle(index, recycleEditor.value) : pass.removeRecycle(index)
                        Connections {
                            target: pass
                            onRecycleChanged: recycleToggler.checked = pass.hasRecycle(index)
                        }

                    }

                    ROWidgets.DoubleInput {
                        id: recycleEditor
                        anchors.right: recycleUnits.left
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: TextInput.AlignRight
                        height: parent.height-3
                        width: 50
                        value: app.units.convertFlowUnits(pass.recycle(index), ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits)
                        onInputChanged: pass.addRecycle(index, app.units.convertFlowUnits(changedValue, app.units.flowUnits, ROUnits.DEFAULT_FLOW_UNITS))
                        Connections {
                            target: pass
                            onRecycleChanged: recycleEditor.value = app.units.convertFlowUnits(pass.recycle(index), ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits)
                        }

                    }

                    Text {
                        id: recycleUnits
                        anchors.right: recycleRightSpacer.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                        font.italic: true
                        horizontalAlignment: Text.AlignLeft
                        width: 30
                    }

                    ROWidgets.Button {
                        id: recycleRightSpacer
                        anchors.right: parent.right
                        width: parent.height-3
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        textFont.pixelSize: 22
                        textLeftMargin: 2
                        onClicked: pass.addRecycle(index, pass.feed.rate - pass.permeate.rate) // = concentrate rate

                        Image {
                            anchors.fill: parent
                            anchors.margins: 2
                            smooth: true
                            source: "../images/recycle-max.png"
                        }

                        tooltip: app.translator.emptyString + qsTr("Max")
                    }
                }
            }

            Repeater {
                id: passRecyclesRepeater
                model: passIndex
                delegate: recyclesDelegate
            }

            Item {
                id: originalFeedRow
                anchors.left: parent.left
                anchors.right: parent.right
                visible: __passHasIncomingRecycles()
                height: 20
                property real originalFeed: pass.feed.rate - pass.recycle(1).rate // TODO!!!!!!!!?????????????????? че за единица?? посчитать все потоки, не только 1

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + qsTr("Original feed flow (F%1):").arg(passIndex+1)
                    font.italic: true
                }

                Text {
                    id: originalFeedRowValue
                    anchors.right: originalFeedRowUnits.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height-3
                    width: 50
                    text: app.units.convertFlowUnits(originalFeedRow.originalFeed, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2)
                    font.italic: true
                    // KeyNavigation.backtab: passFlowFactorInput
                }

                Text {
                    id: originalFeedRowUnits
                    anchors.right: parent.right
                    anchors.rightMargin: parent.height-3
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                    font.italic: true
                    horizontalAlignment: Text.AlignLeft
                    width: 30
                }
            }

            Item {
                id: stageFeedRow
                anchors.left: parent.left
                anchors.right: parent.right
                visible: __passHasIncomingRecycles() || pass.hasSelfRecycle || pass.hasBlendPermeate
                height: 20
                property real stageFeed: pass.feed.rate + (pass.hasSelfRecycle ? pass.selfRecycle : 0.0)

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + qsTr("Feed on first stage (SF%1):").arg(passIndex+1)
                    font.italic: true
                }

                Text {
                    id: feedOnFirstStageValue
                    anchors.right: feedOnFirstStageUnits.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height-3
                    width: 50
                    text: app.units.convertFlowUnits(stageFeedRow.stageFeed, ROUnits.DEFAULT_FLOW_UNITS, app.units.flowUnits).toFixed(2)
                    font.italic: true
                    // KeyNavigation.backtab: passFlowFactorInput
                }

                Text {
                    id: feedOnFirstStageUnits
                    anchors.right: parent.right
                    anchors.rightMargin: parent.height-3
                    anchors.verticalCenter: parent.verticalCenter
                    text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits)
                    font.italic: true
                    horizontalAlignment: Text.AlignLeft
                    width: 30
                }
            }
        }

        Item {
            id: stageContainer
            width: parent.width
            height: stageConf.height +  stageBar.height / 2 + 30

            Rectangle {
                id: border
                border.color: "grey"
                border.width: 1
                color: "white"
//                radius: 3
                anchors.fill: parent
                anchors.topMargin: stageBar.height / 2
            }

            ParametersBar {
                id: stageBar
                label: app.translator.emptyString + qsTr("Stage:")
                z: 1
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 10
                elementsCount: pass.stageCount
                elementsCountMax: 4
                elementsCountMin: 1
                selectedIndex: pass.stageIndex(selectedStage)
                onAdd: pass.addStage(index)
                onRemove: pass.removeStage(index);
                onSelect: selectedStage = pass.stage(index);
            }

            ROConfStage {
                id: stageConf
                stage: selectedStage
                anchors.top: stageBar.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 15
            }

        }
    }
}

