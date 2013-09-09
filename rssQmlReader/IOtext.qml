// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

TextInput {
    font.pixelSize: 18
    color:"red";selectionColor: "blue"
    width: parent.width-4
    anchors.centerIn: parent
    focus: true
    text:""
}
