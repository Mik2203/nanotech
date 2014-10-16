import QtQuick 1.1

import ROElement 1.0

import "../common"
import "../util"

Column {
    id: elementData
    property ROElement element
    spacing: 10

    ElementTitle { text: app.translator.emptyString + qsTr("Element %1").arg(stage.elementIndex(element)+1) }

    Common { element: elementData.element }
    Hydrodynamics { element: elementData.element }
    Solubility { element: elementData.element }
    Scaling { element: elementData.element }
}
