import QtQuick 1.1

import "../util"
import "../common"

Column {
    Row {
        TableRowTitle { title: app.translator.emptyString + qsTr("Rejection") }
        TableRowUnits { units: "%" }
        TableRowValue { value: (1.0 - sys.permeate.solutes.totalValueMgl / sys.feed.solutes.totalValueMgl) * 100.0 }
    }

    Item { // spacer
        height: _ROW_HEIGHT / 2
        width: 1
        Text {
            text: sys.concentrate.solutes.totalValueMgl
        }
    }

    Row {
        FlowSolubilityTitles {}
        FlowSolubilityValues { title: qsTr("Feed"); flow: sys.feed }
        FlowSolubilityValues { title: qsTr("Adj. feed"); flow: sys.adjustedFeed }
        FlowSolubilityValues { title: qsTr("Stage 1 feed"); flow: sys.firstPass.firstStage.feed }
        FlowSolubilityValues { title: qsTr("Permeate"); flow: sys.permeate }
        FlowSolubilityValues { title: qsTr("Concentrate"); flow: sys.concentrate }
    }

//    FlowSolubility {
////        function flows() {
////            return [
////                        { title: qsTr("Feed"), flow: sys.feed },
////                        { title: qsTr("Adj. feed"), flow: sys.adjustedFeed },
////                        { title: qsTr("Stage 1 feed"), flow: sys.firstPass.firstStage.feed },
////                        { title: qsTr("Permeate"), flow: sys.permeate },
////                        { title: qsTr("Concentrate"), flow: sys.concentrate }
////                    ]
////        }
//        property int flowsCount: 5

//        function flow(index) {
//            return [sys.feed,
//                    sys.adjustedFeed,
//                    sys.firstPass.firstStage.feed,
//                    sys.permeate,
//                    sys.concentrate
//                    ][index];
//        }

//        function title(index) {
//            return [qsTr("Feed"),
//                    qsTr("Adj. feed"),
//                    qsTr("Internet"),
//                    qsTr("Messages"),
//                    qsTr("Music"),
//                    qsTr("Call")
//                    ][index];
//        }
//    }
}
