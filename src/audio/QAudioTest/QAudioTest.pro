#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T19:39:19
#
#-------------------------------------------------

# Module of Qt that is need to import
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Program version
VERSION = 1.0.0
# Program Name
TARGET = QAudioTest

TEMPLATE = app

# Windows
win32: {
    INCLUDEPATH += $$_PRO_FILE_PWD_/../AudioPackage/include/portaudio/
    INCLUDEPATH += $$_PRO_FILE_PWD_/../AudioPackage/include/

    # Release config
    CONFIG(release, release|debug) {
        CONFIG_PATH = Release
    }
    # Debug config
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
# Unix (Linux and OsX)
unix: {
    LIBS += -lportaudio
    LIBS += -lopus
}

# Destination directory of the program
DESTDIR = ./

# Destination directory of all ressources necessary for compilation
MOC_DIR = ../.moc/
OBJECTS_DIR = ../.obj/
UI_DIR = ../.ui/
UI_HEADERS_DIR = ../.ui/header/
UI_SOURCES_DIR = ../.ui/src/

# #define
DEFINES += AUDIO_USE_FLOAT

SOURCES += main.cpp\
        mainwindow.cpp \
    audiostream.cpp \
    audiosample.cpp \
    encodedsample.cpp \
    audiosocket.cpp

HEADERS  += mainwindow.h \
    qsynchronizedqueue.h \
    audiostream.h \
    audiosample.h \
    singleton.h \
    encodedsample.h \
    audiosocket.h
