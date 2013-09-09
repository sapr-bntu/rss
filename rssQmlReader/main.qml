// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import Qt 4.7

Rectangle {
    id: mainWin
    width: 800
    height: 500
    color: "Azure"
    radius: 5
    visible: true
    property string currentFeed: ""
    property bool loading: feedModel.status == XmlListModel.Loading

    FeedModel{id: feedModel
    }

    TreeView{id: treeView
        width: 200
        height: 400
    }

    RssNews{id: rssNews
        x: treeView.width
        y: 0
        width: mainWin.width - treeView.width
        height: mainWin.height
    }
    Buttons {id: buttonExit
        y: treeView.height+10+(mainWin.height-(treeView.height+10))/2
        width: treeView.width
        height: (mainWin.height-(treeView.height+10))/2
        Text {
            id: buttonExitText
            anchors.centerIn: parent;
            text: "Exit"
        }
        MouseArea {id: buttonmouseArea
            anchors.fill: parent
            onClicked: {treeModel.quite();}
        }
    }
    Buttons {id: buttonAdd
        y: treeView.height+5
        width: treeView.width
        height: (mainWin.height-(treeView.height+10))/2
        Text {
            id: buttonAddLabel
            text: "Edit feeds"
            anchors.centerIn: parent;
        }
        MouseArea {
            id: buttonAddmouseArea
            anchors.fill: parent
            onClicked:
            {
                if (mouse.button === Qt.LeftButton)
                    mainWin.state = "StateAddFeed"
            }
        }
    }

    Buttons {id: buttonReturn
        y: 450
        width: 150
        height: 50
        opacity: 0
        Text {
            id: buttonReturnLabel
            text: "Return to news"
            anchors.centerIn: parent;
        }
        MouseArea {
            id: buttonReturnmouseArea
            x: 0
            y: 0
            width: 100
            height: 30
            anchors.fill: parent
            onClicked:
            {
                if (mouse.button === Qt.LeftButton)
                    mainWin.state = "StateMainWinow"
            }
        }
    }

    Row{id: addRow
        x: 20
        y: mainWin.height/4
        height: 30
        spacing: 20
        opacity: 0
        Text
        {
            text: "Add Feed: "
        }
        IORect {
            id: feedinputRect //Имя строки ввода
            width: 250
            height: 30


            IOtext {

                id: feedinput
                objectName: "feedinput"

            }
        }
        Text
        {
            text: "Name: "
        }
        IORect {
            id: feedNameinputRect //Имя строки ввода
            width: 80
            height: 30

            IOtext {
                id: feedNameinput
                objectName: "feedNameinput"
            }
        }

        Text
        {
            text: "To Category "
        }
        IORect {
            id: feedCategoryinputRect //Имя строки ввода
            width: 80
            height: 30

            IOtext {
                id: feedCategoryinput
                objectName: "feedCategoryinput"
            }
        }
        Buttons{
            id: addOk
            width: 90
            height: parent.height
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                     if (mouse.button === Qt.LeftButton)
                         window.addFeed();
                }
            }
        }
    }

    Row{id: deleteRow
        x: 20
        y:  mainWin.height/2
        height: 30
        width: addRow.width
        spacing: 20
        opacity: 0
        Text
        {
            text: "Delete Feed, called: "
        }
        IORect {
            id: nameinputRect //Имя строки ввода
            width: 350
            height: 30

            IOtext {
                id: nameinput
                objectName: "nameinput"
            }
        }

        Buttons{
            id: deleteOk
            width: 90
            height: parent.height
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    if (mouse.button === Qt.LeftButton)
                    {
                        window.deleteFeed();
                    }
                }
            }
        }
    }
    Row{id: addcategoryRow
        x: 20
        y:  mainWin.height/2+mainWin.height/5
        height: 30
        width: addcategoryRow.width
        spacing: 20
        opacity: 0
        Text
        {
            text: "Add Category "
        }
        IORect {
            id: nameinputRect1 //Имя строки ввода
            width: 350
            height: 30

            IOtext {
                id: nameinput1
                objectName: "nameinput1"
            }
        }

        Buttons{
            id: addcatOk
            width: 90
            height: parent.height
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    if (mouse.button === Qt.LeftButton)
                    {
                        window.addCategory();
                    }
                }
            }
        }
    }
    Row{id: delcategoryRow
        x: 20
        y:  mainWin.height/2+mainWin.height/3
        height: 30
        width: delcategoryRow.width
        spacing: 20
        opacity: 0
        Text
        {
            text: "Delete Category "
        }
        IORect {
            id: nameinputRect2 //Имя строки ввода
            width: 350
            height: 30
            IOtext {
                id: nameinput2
                objectName: "nameinput2"
            }
        }

        Buttons{
            id: delcatOk
            width: 90
            height: parent.height
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    if (mouse.button === Qt.LeftButton)
                    {
                        window.deleteCategory();
                    }
                }
            }
        }
    }
    Rectangle{id: justRect
        height: 30
        width:mainWin.width
        border.color: "black"
        border.width: 2
        color: "Blue"
        opacity: 0
        Text
        {
            anchors.centerIn: parent
            font.bold: true
            font.pixelSize: parent.height-4
            color: "White"
            text:"Feeds edit page"
        }
    }

    states: [
        State {
            name: "StateMainWinow"
            PropertyChanges {
                target: mainWin
                width: 800
                height: 500
            }

            PropertyChanges {
                target: treeView
                opacity: 1
            }

            PropertyChanges {
                target: rssNews
                opacity: 1

            }

            PropertyChanges {
                target: buttonAdd
                opacity: 1

            }

            PropertyChanges {
                target: buttonReturn
                opacity: 0

            }
            PropertyChanges{
                target: buttonExit
                opacity:1
            }

        },
        State {
            name: "StateAddFeed"
            PropertyChanges {
                target: mainWin
                width: 800
                height: 500
            }

            PropertyChanges {
                target: treeView
                opacity: 0
            }

            PropertyChanges {
                target: rssNews
                opacity: 0

            }

            PropertyChanges {
                target: buttonAdd
                opacity: 0

            }

            PropertyChanges {
                target: buttonReturn
                opacity: 1

            }

            PropertyChanges{
                target: addRow
                opacity: 1
            }

            PropertyChanges{
                target: deleteRow
                opacity: 1
            }

            PropertyChanges{
                target: justRect
                opacity: 1
            }
            PropertyChanges{
                target: addcategoryRow
                opacity: 1
            }
            PropertyChanges{
                target: delcategoryRow
                opacity: 1
            }
            PropertyChanges{
                target: buttonExit
                opacity:0
            }
        }


    ]

}



