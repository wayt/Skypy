#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T19:53:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QVideoTest
TEMPLATE = app

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc

SOURCES += main.cpp\
        widget.cpp \
    videoreader.cpp \
    videosocket.cpp \
    videoviewer.cpp

HEADERS  += widget.h \
    videoreader.h \
    videosocket.h \
    videoviewer.h
