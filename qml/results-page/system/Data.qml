// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import ROPass 1.0

import "../common"
import "../util"

Column { // SYSTEM
    objectName: "systemDetails"
    ElementTitle { text: app.translator.emptyString + qsTr("System details"); objectName: "systemDetailsHeader" }
    spacing: 10

    Common { objectName: "systemDetailsCommon" }
    Hydrodynamics { objectName: "systemDetailsHydrodynamics" }
    Solubility { element: sys; objectName: "systemDetailsSolubility" }
    Scaling { element: sys; objectName: "systemDetailsScaling" }
}
