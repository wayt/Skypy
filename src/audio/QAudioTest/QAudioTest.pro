#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T19:39:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0
TARGET = QAudioTest
TEMPLATE = app

win32: {
    INCLUDEPATH += ../AudioPackage/include/portaudio/
    INCLUDEPATH += ../AudioPackage/include/

    LIBS += ../AudioPackage/portaudio_x64.lib
    LIBS += ../AudioPackage/opus.lib
}
unix: {
    LIBS += -lportaudio
    LIBS += -lopus
}

DESTDIR = ./
MOC_DIR = ../.moc/
OBJECTS_DIR = ../.obj/
UI_DIR = ../.ui/
UI_HEADERS_DIR = ../.ui/header/
UI_SOURCES_DIR = ../.ui/src/

SOURCES += main.cpp\
        mainwindow.cpp \
    audiostream.cpp \
    audiosample.cpp \
    playaudio.cpp \
    encodedsample.cpp

HEADERS  += mainwindow.h \
    qsynchronizedqueue.h \
    audiostream.h \
    audiosample.h \
    singleton.h \
    playaudio.h \
    encodedsample.h
