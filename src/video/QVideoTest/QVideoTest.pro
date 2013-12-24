#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T19:53:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0
TARGET = QVideoTest
TEMPLATE = app

win32: {
    INCLUDEPATH += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/include/

    CONFIG(release, release|debug) {
        CONFIG_PATH = Release
    }

    CONFIG(debug, release|debug) {
        CONFIG_PATH = Debug
    }

    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/opencv_core246.lib
    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/opencv_highgui246.lib
    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/opencv_imgproc246.lib
}
unix: {
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
}

SOURCES += main.cpp\
        widget.cpp \
    videoreader.cpp \
    videosocket.cpp \
    videoviewer.cpp

HEADERS  += widget.h \
    videoreader.h \
    videosocket.h \
    videoviewer.h
