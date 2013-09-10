// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Rectangle {
    property alias nameobj : txt.objectName

    width: 80
    height: 30

    border.color: "black"
    border.width: 2
    //цвет строки ввода
    color: "white"
    radius: 7
    TextInput {
        id:txt
        width: parent.width-4
        anchors.centerIn: parent
        focus: true
        font.pixelSize: 18
        color:"red";
        selectionColor: "blue"
        text:""

    }
}
