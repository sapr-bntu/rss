// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Image {
    id: container
    property bool on: false

    source: "qrc:/images/busy.png"; visible: container.on

    NumberAnimation on rotation {
        running: container.on; from: 0; to: 360; loops: Animation.Infinite; duration: 1200
    }
}
