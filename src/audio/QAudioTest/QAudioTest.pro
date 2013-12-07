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
    INCLUDEPATH += $$_PRO_FILE_PWD_/../AudioPackage/include/portaudio/
    INCLUDEPATH += $$_PRO_FILE_PWD_/../AudioPackage/include/

    CONFIG(release, release|debug) {
        CONFIG_PATH = Release
    }
    CONFIG(debug, release|debug) {
        CONFIG_PATH = Debug
    }

    LIBS += $$_PRO_FILE_PWD_/../AudioPackage/$$CONFIG_PATH/portaudio_x64.lib

    LIBS += $$_PRO_FILE_PWD_/../AudioPackage/$$CONFIG_PATH/opus.lib
    LIBS += $$_PRO_FILE_PWD_/../AudioPackage/$$CONFIG_PATH/celt.lib
    LIBS += $$_PRO_FILE_PWD_/../AudioPackage/$$CONFIG_PATH/silk_common.lib
    LIBS += $$_PRO_FILE_PWD_/../AudioPackage/$$CONFIG_PATH/silk_fixed.lib
    LIBS += $$_PRO_FILE_PWD_/../AudioPackage/$$CONFIG_PATH/silk_float.lib
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
