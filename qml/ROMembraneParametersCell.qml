// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Text {
    anchors.verticalCenter: parent.verticalCenter
    width: 80
    clip: true
    elide: Text.ElideRight
    color: stage.membraneId === model.id ? "white" : "black"
}
