// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

FocusScope {
    id: comboBox
    property string initialText
    property int maxHeight
    property int selectedItem: 0
    property string currText: initialText
    property variant listModel
    signal expanded
    signal closed

    ComboBoxButton {
        id: comboBoxButton
        width: parent.width
        height: parent.height
        source: "qrc:/images/arrow-down.png"
        borderColor: "black"
        radius: 10
        margin: 5
        borderWidth: 2
        text: initialText

        onClicked: {
            if (listView.height == 0) {
                listView.height = Math.min(maxHeight, listModel.count*comboBoxButton.height)
                comboBox.expanded()}
            else {
                listView.height = 0
                comboBox.closed()}
        }
    }

    Component {
        id: comboBoxDelegate

        Rectangle {
            id: delegateRectangle
            width: comboBoxButton.width
            height: comboBoxButton.height
            color: "#00000000"
            radius: comboBoxButton.radius
            border.width: comboBoxButton.borderWidth
            border.color: comboBoxButton.borderColor

            Item {
                anchors.left: parent.left
                width: parent.width-parent.height-2*comboBoxButton.margin
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Text {
                    color: index == listView.currentIndex ? "Yellow" : "Black"
                    anchors.centerIn: parent
                    text: value
                    font.bold: true
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    listView.height = 0
                    listView.currentIndex = index
                    comboBox.selectedItem = index
                    comboBoxButton.text = value
                    currText = value
                    comboBox.closed()
                }
            }
        }
    }

    ListView {
        id: listView
        anchors.top: comboBoxButton.bottom
        anchors.left: comboBoxButton.left
        width: parent.width
        height: 0
        clip: true
        model: listModel
        delegate: comboBoxDelegate
        currentIndex: selectedItem

        ScrollBar {
            scrollArea: listView; height: listView.height; width: 8
            anchors.right: listView.right
        }

        Behavior on height {

            NumberAnimation {
                id: animateHeight
                property: "height"
                easing {type: Easing.Linear}
            }
        }
    }
}
