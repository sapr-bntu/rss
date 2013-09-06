// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

ListView {
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



}
