#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T14:34:12
#
#-------------------------------------------------

QT       += testlib\
            network \
            sql \
            webkit \
            xml\
            gui

TARGET = tst_testingprojecttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_testingprojecttest.cpp \
    "RSSReader/SASHA RSS READER/mainwindow.cpp"


DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    "RSSReader/SASHA RSS READER/mainwindow.h" \
    "RSSReader/SASHA RSS READER/connection.h"


INCLUDEPATH += "RSSReader/SASHA RSS READER/"



OTHER_FILES +=

