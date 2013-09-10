// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    anchors.left: parent.bottom
    color:  buttonmouseArea ? "#d40000" : "#6d5e5e"
    radius: 7
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#6d5e5e";
        }
        GradientStop {
            position: 1.00;
            color: "#ffffff";
        }
    }
    z: 2
    border.width: 3
    border.color: "#a3a3b3"
}
