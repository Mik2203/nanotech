// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

import "../widgets" as ROWidgets

ROWidgets.TextInput {
    validator: DoubleValidator {}
    property double fixedPrecision: app.visibleDecimals
    onValueChanged: _inputText = parseFloat(value).toFixed(parseFloat(value) < 1000 ? fixedPrecision : 0);
    onInputChanged: {
        if (value != undefined && value != changedValue)
            _inputText = parseFloat(value).toFixed(parseFloat(value) < 1000 ? fixedPrecision : 0);
    }
}
