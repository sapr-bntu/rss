// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

ListView {
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
