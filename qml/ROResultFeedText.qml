// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property real value
    width: label.width
    color: "#cfcfcf"
    height: 18

    Text { id: label; text: value.toFixed(2); height: 18 }
}
