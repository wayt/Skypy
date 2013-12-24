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
    INCLUDEPATH += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/include/portaudio/
    INCLUDEPATH += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/include/

    # Release config
    CONFIG(release, release|debug) {
        CONFIG_PATH = Release
    }
    # Debug config
    CONFIG(debug, release|debug) {
        CONFIG_PATH = Debug
    }

    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/portaudio_x64.lib

    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/opus.lib
    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/celt.lib
    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/silk_common.lib
    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/silk_fixed.lib
    LIBS += $$_PRO_FILE_PWD_/../../shared/WindowsPackage/$$CONFIG_PATH/silk_float.lib
}
# Unix (Linux and OsX)
unix: {
    LIBS += -lportaudio
    LIBS += -lopus
}

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
