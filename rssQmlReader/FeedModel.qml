// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

XmlListModel {
    source: "http://"+mainWin.currentFeed
    query: "/rss/channel/item"

    XmlRole { name: "title"; query: "title/string()" }
    XmlRole { name: "link"; query: "link/string()" }
    XmlRole { name: "description"; query: "description/string()" }
}
