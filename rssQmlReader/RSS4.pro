#-------------------------------------------------
#
# Project created by QtCreator 2013-05-17T09:26:57
#
#-------------------------------------------------

QT       += core gui declarative\
            sql\
            network \
            webkit \
            xml

TARGET = RSS4
TEMPLATE = app


SOURCES += main.cpp \
    treemodel.cpp \
    mainwindow.cpp

HEADERS  += \
    treemodel.h \
    mainwindow.h

OTHER_FILES += \
    main.qml \
    ScrollBar.qml \
    NewsDelegate.qml \
    BusyIndicator.qml \
    ComboBox.qml \
    ComboBoxButton.qml \
    Buttons.qml \
    FeedModel.qml \
    TreeView.qml \
    RssNews.qml

RESOURCES += \
    qrc.qrc

