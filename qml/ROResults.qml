// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import ROSystem 1.0
//import ROSystemController 1.0
import ROStage 1.0
import ROPass 1.0

import ROSolutes 1.0
import ROSoluteModel 1.0
import ROWaterTypeModel 1.0
import ROUnits 1.0

import "widgets" as ROWidgets




Item {
    id: container
    property int colNumData: sysColNum()
    property real rowHeight: 18
    property real colWidthData: 80
    property real colWidthParam: 150
    property real colWidthUnits: 50
    property real solutesHeaderOffset: 20
    property color borderColor: "grey"

    property bool showFeed: true
    property bool showPermeate: true
    property bool showConcentrate: true

    property int showFieldNum: getShowFieldNum()

    height: mainColumn.height
    width: mainColumn.width

    property variant expandFlags: {}


    Component {
        id: nullWidthStub
        Item {
            width: 0
        }
    }

    function sysColNum() {
        var colNum = 1; // SYSTEM COL
        if (expandFlags[sys])
            for (var pIdx=0; pIdx<sys.passCount; ++pIdx) {
                colNum += passColNum(pIdx);
            }
        return colNum;
    }

    function passColNum(pIdx) {
        var colNum = 1; // PASS COLL
        if (expandFlags[sys.pass(pIdx)]) {
            for (var sIdx=0; sIdx<sys.pass(pIdx).stageCount; ++sIdx) {
                colNum += stageColNum(pIdx, sIdx); // STAGE COL
            }
        }
        return colNum;
    }

    function stageColNum(pIdx, sIdx) {
        var colNum = 1; // STAGE COLL
        if (expandFlags[sys.pass(pIdx).stage(sIdx)])
            colNum += sys.pass(pIdx).stage(sIdx).elementsPerVesselCount;
        return colNum;
    }

    function getShowFieldNum() {
        if (showFeed && showPermeate && showConcentrate)
            return 3;
        else if((showFeed && showPermeate) ||
                (showFeed && showConcentrate) ||
                (showPermeate && showConcentrate))
            return 2;
        else if (showFeed || showPermeate || showConcentrate)
            return 1;
        return 0;
    }

    function setAllExpanded(expanded) {
        var temp = expandFlags

        temp[sys] = expanded;
        for (var pIdx=0; pIdx<sys.passCount; ++pIdx) {
            temp[sys.pass(pIdx)] = expanded;
            for (var sIdx=0; sIdx<sys.pass(pIdx).stageCount; ++sIdx) {
                temp[sys.pass(pIdx).stage(sIdx)] = expanded;
            }
        }
        expandFlags = temp;
    }

    Column {
        id: mainColumn

        Row { // SYSTEM HEADER
            Item {
                height: rowHeight;
                width: colWidthParam + colWidthUnits

                Row {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    //anchors.leftMargin: 5
                    spacing: 5

                    Text {
                        text: app.translator.emptyString + qsTr("Show flows: ")
                    }

                    ROWidgets.CheckBox {
                        checkedColor: "grey"
                        checked: showFeed
                        onToggle: showFeed = state
                    }
                    ROWidgets.CheckBox {
                        checkedColor: "#018CD1"
                        checked: showPermeate
                        onToggle: showPermeate = state
                    }
                    ROWidgets.CheckBox {
                        checkedColor: "red"
                        checked: showConcentrate
                        onToggle: showConcentrate = state
                    }
                }
            } //Spacer

            ROResultsHeaderButton {
                id: systemHeader
                text: app.translator.emptyString + qsTr("System")
                width: expandFlags[sys] ? colWidthData * colNumData : colWidthData;
                toggled: expandFlags[sys]  ? true : false
                onToggledChanged: {
                    if (toggled != expandFlags[sys]) {
                        var temp = expandFlags
                        temp[sys] = systemHeader.toggled
                        expandFlags = temp
                    }
                }
                Connections { target: container; onExpandFlagsChanged: systemHeader.toggled = expandFlags[sys]  ? true : false; }
            }

            ROWidgets.Button {
                id: expandAllButton
                width: height;
                height: rowHeight;
//                radius: 0
                onClicked: setAllExpanded(true)

                Image {
                    id: arrow1
                    source: "../images/spinbox_down.png"
                    rotation: -90 //expandAllButton.toggled ? 90 : -90
                    //Behavior on rotation { NumberAnimation { duration: 300 } }
                    smooth: true
                    anchors.right: parent.right
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter
                    width: 7
                    height: 5
                }

                Image {
                    id: arrow2
                    source: "../images/spinbox_down.png"
                    rotation: -90 //expandAllButton.toggled ? 90 : -90
                    //Behavior on rotation { NumberAnimation { duration: 300 } }
                    smooth: true
                    anchors.right: arrow1.left
                    //anchors.rightMargin: 3
                    anchors.verticalCenter: parent.verticalCenter
                    width: 7
                    height: 5
                }
            }
        }

        Row { // SYSTEM TEMPERATURE
            // spacing: 5
            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Temperature"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle {
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + unitsText.temperatureUnitText(app.units.temperatureUnits); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
            }

            ROResultText {
                width: expandFlags[sys] ? colWidthData * colNumData : colWidthData
                value: app.units.convertTemperatureUnits(sys.temperature, ROUnits.DEFAULT_TEMPERATURE_UNITS, app.units.temperatureUnits);
            }
        }

        Row { // SYSTEM WATER TYPE
            // spacing: 5
            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Water type"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle { //SYSTEM SPACER
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
            }

            Rectangle {
                //width: 30
                width: expandFlags[sys] ? colWidthData * colNumData : colWidthData
                height: rowHeight
                border.color: borderColor
                clip: true
                Text { text: db.waterTypes.get(sys.waterTypeIndex, "name"); anchors.right: parent.right;
                    anchors.rightMargin: 2; anchors.verticalCenter: parent.verticalCenter }
            }
        }

        Row { // PASS HEADER
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0

            Item { height: rowHeight; width: colWidthParam + colWidthUnits } //Spacer
            Repeater {
                id: expandPass
                model: sys.passCount

                ROResultsHeaderButton {
                    id: passHeader
                    text: app.translator.emptyString + qsTr("Pass %1").arg(index+1)
                    width: colWidthData * passColNum(index)
                    toggled: expandFlags[sys.pass(index)] ? true : false
                    onToggledChanged: {
                        if (passHeader.toggled != expandFlags[sys.pass(index)]) {
                            var temp = expandFlags
                            temp[sys.pass(index)] = toggled
                            expandFlags = temp
                        }
                    }
                    Connections { target: container; onExpandFlagsChanged: passHeader.toggled = expandFlags[sys.pass(index)]  ? true : false; }
                }

            }
            Rectangle { //SYSTEM SPACER
                width: colWidthData
                height: rowHeight
                border.color: borderColor
            }
        }

        Row { // PASS FLOW FACTORS
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0
            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Flow Factor"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle { // NO UNITS
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
            }

            Repeater {
                model: sys.passCount
                ROResultText {
                    property ROPass pass: sys.pass(index)
                    width: colWidthData * passColNum(index)
                    value: pass.flowFactor;
                }
            }
            Rectangle { // SYSTEM SPACER
                width: colWidthData
                height: rowHeight
                border.color: borderColor
            }
        }

        Row { // PASS POWER
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0
            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Power"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle {
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("kW"); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
            }


            Repeater {
                model: sys.passCount
                ROResultText {
                    property ROPass pass: sys.pass(index)
                    width: colWidthData * passColNum(index)
                    value: pass.power;
                }
            }

            ROResultText { value: sys.power;  }
        }

        Row { // PASS SPECIFIC ENERGY
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0
            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Specific energy"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle {
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("kWh/m³"); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
            }

            Repeater {
                model: sys.passCount

                ROResultText {
                    property ROPass pass: sys.pass(index)
                    width: colWidthData * passColNum(index)
                    value: pass.specificEnergy;
                }
            }
            ROResultText { value: sys.specificEnergy; }
        }




        Row { // STAGE HEADER
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0
            Item { height: rowHeight; width: colWidthParam + colWidthUnits } //Spacer
            Repeater {
                model: sys.passCount
                delegate: Row {
                    //                    visible: expandFlags[pass] ? true : false
                    //                    height: visible ? rowHeight : 0
                    property ROPass pass: sys.pass(index)
                    property int pIdx: index

                    Repeater {
                        model: pass.stageCount

                        ROResultsHeaderButton {
                            id: stageHeader
                            width: colWidthData * stageColNum(pIdx, index)
                            text: app.translator.emptyString + qsTr("Stage %1").arg(index+1)
                            visible: expandFlags[pass] ? true : false
                            toggled: expandFlags[pass.stage(index)] ? true : false
                            onToggledChanged: {
                                if (stageHeader.toggled != expandFlags[pass.stage(index)]) {
                                    var temp = expandFlags
                                    temp[pass.stage(index)] = toggled
                                    expandFlags = temp
                                }
                            }
                            Connections { target: container; onExpandFlagsChanged: stageHeader.toggled = expandFlags[pass.stage(index)]  ? true : false; }
                        }

                    }
                    Rectangle { // PASS SPACER
                        //width: 30
                        width: colWidthData
                        height: rowHeight
                        border.color: borderColor
                    }
                }

            }

            // TODO esli hotya bi odin pass est' - to nado, esli ni odnogo, to net
            Rectangle {
                //width: 30
                width: colWidthData
                height: rowHeight
                border.color: borderColor
            }
        }


        Row { // STAGE MEMBRANES
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0

            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Membrane"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle { //Spacer
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
            }

            Repeater {
                model: sys.passCount
                delegate: Row {
                    height: rowHeight
                    property ROPass pass: sys.pass(index)
                    property int pIdx: index

                    Row {
                        height: rowHeight
                        visible: expandFlags[pass] ? true : false

                        Repeater {
                            model: pass.stageCount

                            Rectangle {
                                property ROStage stage: pass.stage(index)
                                width: colWidthData * stageColNum(pIdx, index)
                                height: rowHeight
                                border.color: borderColor
                                Text {text: stage.membrane.seriesAndModel; anchors.centerIn: parent }
                            }
                        }
                    }

                    Rectangle { // PASS SPACER
                        width: colWidthData
                        height: rowHeight
                        border.color: borderColor
                    }

                }

            }

            // TODO esli hotya bi odin pass est' - to nado, esli ni odnogo, to net
            Rectangle {
                //width: 30
                width: colWidthData
                height: rowHeight
                border.color: borderColor
            }
        }


        Row { // STAGE NUMBER OF ELEMENTS
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0

            Rectangle {
                width: colWidthParam
                height: rowHeight
                border.color: borderColor
                Text {text: app.translator.emptyString + qsTr("Total elements count"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
            }

            Rectangle { //Spacer
                width: colWidthUnits
                height: rowHeight
                border.color: borderColor
            }

            Repeater {
                model: sys.passCount
                delegate: Row {
                    height: rowHeight
                    property ROPass pass: sys.pass(index)
                    property int pIdx: index

                    Row {
                        height: rowHeight
                        visible: expandFlags[pass] ? true : false

                        Repeater {
                            model: pass.stageCount

                            Rectangle {
                                property ROStage stage: pass.stage(index)
                                width: colWidthData * stageColNum(pIdx, index)
                                height: rowHeight
                                border.color: borderColor
                                Text {text: stage.elementsCount; anchors.centerIn: parent }
                            }
                        }
                    }

                    Rectangle { // PASS SPACER
                        width: colWidthData
                        height: rowHeight
                        border.color: borderColor
                    }

                }

            }

            // TODO esli hotya bi odin pass est' - to nado, esli ni odnogo, to net
            Rectangle {
                //width: 30
                width: colWidthData
                height: rowHeight
                border.color: borderColor
            }
        }


        Row { // ELEMENTS HEADER
            visible: expandFlags[sys] ? true : false
            height: visible ? rowHeight : 0
            Item { height: rowHeight; width: colWidthParam + colWidthUnits } //Spacer
            Repeater {
                model: sys.passCount
                delegate: Row {
                    property ROPass pass: sys.pass(index)
                    property int pIdx: index

                    Repeater {
                        model: pass.stageCount

                        delegate: Row {
                            property ROStage stage: pass.stage(index)
                            visible: expandFlags[pass] ? true : false
                            Repeater {
                                model: stage.elementsPerVesselCount
                                delegate: Rectangle {
                                    visible: expandFlags[stage] ? true : false
                                    width: visible ? colWidthData : 0
                                    height: rowHeight
                                    border.color: borderColor
                                    Text {text: app.translator.emptyString + qsTr("El %1").arg(index+1); font.bold: true; anchors.centerIn: parent }
                                }
                            }

                            Rectangle { // STAGE SPACER
                                //width: 30
                                width: colWidthData
                                height: rowHeight
                                border.color: borderColor
                            }
                        }



                    }
                    Rectangle { // PASS SPACER
                        //width: 30
                        width: colWidthData
                        height: rowHeight
                        border.color: borderColor
                    }
                }

            }
            Rectangle { // sys SPACER
                //width: 30
                width: colWidthData
                height: rowHeight
                border.color: borderColor
            }
        }


        Row {
            // spacing: 5
            Column { // VERTICAL PARAMS HEADERS
                width: colWidthParam


                Rectangle {
                    width: colWidthParam
                    height: rowHeight
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Active Area"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }

                Rectangle {
                    width: colWidthParam
                    height: rowHeight
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Avg. Flux"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }

                Rectangle {
                    width: colWidthParam
                    height: rowHeight
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Recovery"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }
                Rectangle {
                    width: colWidthParam
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Flow"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }


                Rectangle {
                    width: colWidthParam
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Pressure"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }
                Rectangle {
                    width: colWidthParam
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Osm. Pressure"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }


                Item { // SOLUTES HEADER
                    width: colWidthData
                    height: rowHeight + solutesHeaderOffset
                    visible: showFieldNum >0 //&& sys.adjustedFeed.solutes.totalValueMgl > 0
                    Text {text: app.translator.emptyString + qsTr("Solutes data");
                        anchors.leftMargin: 5;
                        anchors.left: parent.left;
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2

                    }
                }

                Column { // SOLUTES

                    Rectangle {
                        width: colWidthParam
                        height: showFieldNum*rowHeight
                        visible: showFieldNum > 0
                        border.color: borderColor
                        Text {text: app.translator.emptyString + qsTr("pH"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                    }

                    Rectangle {
                        width: colWidthParam
                        height: showFieldNum*rowHeight
                        visible: showFieldNum > 0
                        border.color: borderColor
                        Text {text: app.translator.emptyString + qsTr("TDS"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                    }

                    ROSoluteModel { id: soluteModel } // to access static fields only

                    Component {
                        id: solutesNameDelegate

                        Rectangle {
                            id: soluteNameCell
                            visible: showFieldNum > 0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))

                            Connections {
                                target: sys.adjustedFeed.solutes
                                onSolutesChanged: soluteNameCell.visible = showFieldNum > 0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))
                            }
                            Connections {
                                target: container
                                onShowFieldNumChanged: soluteNameCell.visible = showFieldNum > 0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))
                            }
                            width: colWidthParam
                            height: showFieldNum*rowHeight
                            border.color: borderColor
                            Text {text: soluteModel.shortNameByIndex(index); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                        }

                    }

                    Repeater {
                        model: ROSolutes.TotalIons
                        delegate: solutesNameDelegate
                    }
                }

                Item { // SCALING HEADER
                    width: colWidthData
                    height: rowHeight + solutesHeaderOffset
                    visible: showFieldNum >0
                    //visible: sys.adjustedFeed.solutes.totalValueMgl > 0
                    Text {text: app.translator.emptyString + qsTr("Scaling data");
                        anchors.leftMargin: 5;
                        anchors.left: parent.left;
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2

                    }
                }

                Rectangle {
                    width: colWidthParam
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("LSI"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }

                Rectangle {
                    width: colWidthParam
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("S&DSI"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }
                Rectangle {
                    width: colWidthParam
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + qsTr("Ionic Strength"); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                }

                Repeater {
                    model: ROSolutes.TotalCompounds

                    Rectangle {
                        width: colWidthParam
                        height: showFieldNum*rowHeight
                        visible: showFieldNum > 0 && (sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index))
                        border.color: borderColor
                        Text {text: soluteModel.shortCompoundNameByIndex(index); anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter }
                    }
                }

            }

            Column { // VERTICAL UNITS HEADERS
                width: colWidthUnits

                Rectangle {
                    width: colWidthUnits
                    height: rowHeight
                    border.color: borderColor
                    Text {text: app.translator.emptyString + unitsText.areaUnitText(app.units.areaUnits); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                }

                Rectangle {
                    width: colWidthUnits
                    height: rowHeight
                    border.color: borderColor
                    Text {text: app.translator.emptyString + unitsText.fluxUnitText(app.units.fluxUnits); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                }

                Rectangle {
                    width: colWidthUnits
                    height: rowHeight
                    border.color: borderColor
                    Text {text: "%"; font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                }
                Rectangle {
                    width: colWidthUnits
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + unitsText.flowUnitText(app.units.flowUnits); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                }


                Rectangle {
                    width: colWidthUnits
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                }
                Rectangle {
                    width: colWidthUnits
                    height: showFieldNum*rowHeight
                    visible: showFieldNum > 0
                    border.color: borderColor
                    Text {text: app.translator.emptyString + unitsText.pressureUnitText(app.units.pressureUnits); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                }


                Item { // SOLUTES HEADER SPACER
                    //width: 30
                    width: colWidthData
                    height: rowHeight + solutesHeaderOffset
                    visible: sys.adjustedFeed.solutes.totalValueMgl > 0
                }

                Column { // SOLUTES

                    Rectangle { // NO UNITS pH
                        width: colWidthUnits
                        height: showFieldNum*rowHeight
                        border.color: borderColor
                    }

                    Rectangle {
                        width: colWidthUnits
                        height: showFieldNum*rowHeight
                        visible: showFieldNum > 0
                        border.color: borderColor
                        Text {text: app.translator.emptyString + qsTr("mg/l"); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                    }

                    Component {
                        id: solutesUnitsDelegate

                        Rectangle {
                            id: soluteUnitCell
                            visible: showFieldNum > 0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))

                            Connections {
                                target: sys.adjustedFeed.solutes
                                onSolutesChanged: soluteUnitCell.visible= showFieldNum > 0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))
                            }
                            Connections {
                                target: container
                                onShowFieldNumChanged: soluteUnitCell.visible = showFieldNum > 0 && (sys.feed.solutes.isIonFilled(index) || sys.adjustedFeed.solutes.isIonFilled(index))
                            }
                            width: colWidthUnits
                            height: showFieldNum*rowHeight
                            border.color: borderColor
                            Text {text: app.translator.emptyString + qsTr("mg/l"); font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                        }

                    }

                    Repeater {
                        model: ROSolutes.TotalIons
                        delegate: solutesUnitsDelegate
                    }
                }

                Item { // SCALING HEADER SPACER
                    //width: 30
                    width: colWidthData
                    height: rowHeight + solutesHeaderOffset
                }

                Rectangle { // NO UNITS LSI
                    width: colWidthUnits
                    height: showFieldNum*rowHeight
                    border.color: borderColor
                }
                Rectangle { // NO UNITS S&DSI
                    width: colWidthUnits
                    height: showFieldNum*rowHeight
                    border.color: borderColor
                }
                Rectangle { // NO UNITS IC
                    width: colWidthUnits
                    height: showFieldNum*rowHeight
                    border.color: borderColor
                }

                Repeater {
                    model: ROSolutes.TotalCompounds

                    delegate: Rectangle {
                        visible: showFieldNum > 0 && (sys.feed.solutes.isSaturated(index) || sys.adjustedFeed.solutes.isSaturated(index))
                        width: colWidthUnits
                        height: showFieldNum*rowHeight
                        border.color: borderColor
                        Text {text: "%"; font.italic: true; anchors.leftMargin: 5; anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter}
                    }
                }
            }

            Repeater { // DATA
                model: sys.passCount
                delegate: Loader {
                    Component { id: passComponent; ROResultsPassData{} }
                    sourceComponent: expandFlags[sys] ? passComponent : nullWidthStub
                }
            }

            ROResultsSysData { }
        }
    }
}
