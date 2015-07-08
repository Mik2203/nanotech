import QtQuick 1.1

import "../util"
import "../common"
import "../util/getFlows.js" as GetFlows


Row {
    ScalingHeader {}
    Column {
        Item { height: _ROW_HEIGHT; width: _VALUE_WIDTH; }
        ScalingData { flows: GetFlows.forElement(sys) }
    }
}
