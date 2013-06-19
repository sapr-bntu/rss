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

    XmlListModel {id: feedModel
        source: "http://"+mainWin.currentFeed
        query: "/rss/channel/item"

        XmlRole { name: "title"; query: "title/string()" }
        XmlRole { name: "link"; query: "link/string()" }
        XmlRole { name: "description"; query: "description/string()" }
    }

    ListView {id: treeView       
        width: 200
        height: 400
        ScrollBar {
            scrollArea: treeView; height: treeView.height; width: 8
            anchors.right: treeView.right
        }
        //Задаем делегата
        delegate: treeDelegate
        //Задаем модель, этот объект позже придет из C++
        model: treeModel
        //Компонент делегата
        Component {
            id: treeDelegate
            Item {
                id: wrapper
                height: 48
                width: treeView.width

                //Полоска для отделения элементов друг от друга
                Rectangle {
                    height: 1
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: "#d0d0d0"
                }
                //"Отбой" слева элементов-потомков
                Item {
                    id: levelMarginElement
                    //Начиная с 6 уровня вложенности не сдвигаем потомков,
                    //так как иначе можно получить очень широкое окно
                    width: (level>5?6:level)*32 + 5
                    anchors.left: parent.left
                }
                //Область для открытия/закрытия потомков.
                //На листьях не виден
                Item {
                    id: nodeOpenElement
                    anchors.left: levelMarginElement.right
                    anchors.verticalCenter: wrapper.verticalCenter
                    height: 48
                    state: "leafNode"
                    Image {
                        id: triangleOpenImage
                        //Отлавливаем нажатие мышкой и открываем/закрываем элемент
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                (isOpened) ? treeModel.closeItem(index) : treeModel.openItem(index);
                            }
                        }
                    }
                    states: [
                        //Лист
                        //Область не видна
                        State {
                            name: "leafNode"
                            when: !hasChildren
                            PropertyChanges {
                                target: nodeOpenElement
                                visible: false
                                width: 0
                            }
                        },
                        //Открытый элемент
                        //Область видна и отображена соответствующая иконка
                        State {
                            name: "openedNode"
                            when: (hasChildren)&&(isOpened)
                            PropertyChanges {
                                target: nodeOpenElement
                                visible: true
                                width: 48
                            }

                            PropertyChanges
                            {
                                target: triangleOpenImage
                                source: "qrc:/images/minus-icon.png"
                            }
                        },
                        //Закрытый элемент
                        //Область видна и отображена соответствующая иконка
                        State {
                            name: "closedNode"
                            when: (hasChildren)&&(!isOpened)
                            PropertyChanges {
                                target: nodeOpenElement
                                visible: true
                                width: 48
                            }
                            PropertyChanges {
                                target: triangleOpenImage
                                source: "qrc:/images/plus-icon.png"
                            }
                        }
                    ]
                }
                //Область для отображения данных элемента
                Text {id: nameTextElement
                    text: name
                    verticalAlignment: "AlignVCenter"
                    anchors.left: nodeOpenElement.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: if(!hasChildren) index == treeView.currentIndex ? "#d40000" : "Black"
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            if(!hasChildren)
                            {
                                mainWin.currentFeed = feed;
                                treeView.currentIndex = index;
                            }
                        }
                    }
                }
            }
        }

        Rectangle {id: buttonExit
            anchors.left: parent.bottom
            y: treeView.height+10+(mainWin.height-(treeView.height+10))/2
            width: treeView.width
            height: (mainWin.height-(treeView.height+10))/2
            color:  buttonExitmouseArea ? "#d40000" : "#6d5e5e"
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
            Text {
                id: buttonExitText
                text: "Exit"
                anchors.centerIn: parent;
            }
            MouseArea {
                id: buttonExitmouseArea
                anchors.fill: parent
                onClicked: {treeModel.quite();}

            }
        }
    }

    ListView {id: rssNews
        x: treeView.width
        y: 0
        width: mainWin.width - treeView.width
        height: mainWin.height
        model: feedModel
        delegate: NewsDelegate{}
        ScrollBar {
            scrollArea: rssNews; height: rssNews.height; width: 8
            anchors.right: rssNews.right
        }
        BusyIndicator
        {
            scale: 2
            on:  mainWin.loading
            anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
        }

    }

    Rectangle {id: buttonAdd
        anchors.left: parent.bottom
        y: treeView.height+5
        width: treeView.width
        height: (mainWin.height-(treeView.height+10))/2
        color: buttonAddmouseArea.pressed ? "#d40000" : "#6d5e5e"
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

    Rectangle {id: buttonReturn
        anchors.left: parent.bottom
        y: 450
        width: 150
        height: 50
        opacity: 0
        color: "#6d5e5e"
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
        Rectangle {
            id: feedinputRect //Имя строки ввода
            width: 250
            height: 30

            border.color: "black"
            border.width: 2

            //цвет строки ввода
            color: "white"

            TextInput {
                id: feedinput
                objectName: "feedinput"
                color: "black";
                selectionColor: "blue"
                font.pixelSize: 12;
                width: parent.width-4
                anchors.centerIn: parent
                focus: true
                text:""
            }
        }
        Text
        {
            text: "Name: "
        }
        Rectangle {
            id: feedNameinputRect //Имя строки ввода
            width: 80
            height: 30

            border.color: "black"
            border.width: 2

            //цвет строки ввода
            color: "white"

            TextInput {
                id: feedNameinput
                objectName: "feedNameinput"
                color: "black";
                selectionColor: "blue"
                font.pixelSize: 12;
                width: parent.width-4
                anchors.centerIn: parent
                focus: true
                text:""
            }
        }

        Text
        {
            text: "To Category "
        }
        Rectangle {
            id: feedCategoryinputRect //Имя строки ввода
            width: 80
            height: 30

            border.color: "black"
            border.width: 2

            //цвет строки ввода
            color: "white"

            TextInput {
                id: feedCategoryinput
                objectName: "feedCategoryinput"
                color: "black";
                selectionColor: "blue"
                font.pixelSize: 12;
                width: parent.width-4
                anchors.centerIn: parent
                focus: true
                text:""
            }
        }
        Rectangle{
            id: addOk
            width: 90
            height: parent.height
            color: buttonOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: buttonOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
                }
                GradientStop {
                    position: 1.00;
                    color: "#ffffff";
                }
            }
            z: 2
            border.width: 3
            border.color: "#a3a3b3"
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                id: buttonOkmouseArea
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
        Rectangle {
            id: nameinputRect //Имя строки ввода
            width: 350
            height: 30

            border.color: "black"
            border.width: 2

            //цвет строки ввода
            color: "white"

            TextInput {
                id: nameinput
                objectName: "nameinput"
                color: "black";
                selectionColor: "blue"
                font.pixelSize: 12;
                width: parent.width-4
                anchors.centerIn: parent
                focus: true
                text:""
            }
        }

        Rectangle{
            id: deleteOk
            width: 90
            height: parent.height
            color: deleteOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: deleteOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
                }
                GradientStop {
                    position: 1.00;
                    color: "#ffffff";
                }
            }
            z: 2
            border.width: 3
            border.color: "#a3a3b3"
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                id: deleteOkmouseArea
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
        Rectangle {
            id: nameinputRect1 //Имя строки ввода
            width: 350
            height: 30

            border.color: "black"
            border.width: 2

            //цвет строки ввода
            color: "white"

            TextInput {
                id: nameinput1
                objectName: "nameinput1"
                color: "black";
                selectionColor: "blue"
                font.pixelSize: 12;
                width: parent.width-4
                anchors.centerIn: parent
                focus: true
                text:""
            }
        }

        Rectangle{
            id: addcatOk
            width: 90
            height: parent.height
            color: deleteOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: deleteOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
                }
                GradientStop {
                    position: 1.00;
                    color: "#ffffff";
                }
            }
            z: 2
            border.width: 3
            border.color: "#a3a3b3"
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                id: addcategmouseArea
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
        Rectangle {
            id: nameinputRect2 //Имя строки ввода
            width: 350
            height: 30

            border.color: "black"
            border.width: 2

            //цвет строки ввода
            color: "white"

            TextInput {
                id: nameinput2
                objectName: "nameinput2"
                color: "black";
                selectionColor: "blue"
                font.pixelSize: 12;
                width: parent.width-4
                anchors.centerIn: parent
                focus: true
                text:""
            }
        }

        Rectangle{
            id: delcatOk
            width: 90
            height: parent.height
            color: deleteOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: deleteOkmouseArea.pressed ? "#d40000" : "#6d5e5e"
                }
                GradientStop {
                    position: 1.00;
                    color: "#ffffff";
                }
            }
            z: 2
            border.width: 3
            border.color: "#a3a3b3"
            Text {
                text: "Ok"
                anchors.centerIn: parent;
            }
            MouseArea {
                id: delcategmouseArea
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
        }


    ]

}



