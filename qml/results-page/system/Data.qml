// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import ROPass 1.0

import "../common"
import "../pass" as PassResults
import "../util"

Column { // SYSTEM
    ElementTitle { text: app.translator.emptyString + qsTr("System") }
    spacing: 10

    Common {}
    Hydrodynamics {}
    Solubility { element: sys }
//    Scaling { element: sys }

    PassResults.Data {}
}
