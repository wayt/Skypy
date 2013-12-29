#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T13:01:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skypy
TEMPLATE = app

# Windows
win32: {
    INCLUDEPATH += $$_PRO_FILE_PWD_/../shared/WindowsPackage/include/portaudio/
    INCLUDEPATH += $$_PRO_FILE_PWD_/../shared/WindowsPackage/include/

    # Release config
    CONFIG(release, release|debug) {
        CONFIG_PATH = Release
    }
    # Debug config
    CONFIG(debug, release|debug) {
        CONFIG_PATH = Debug
    }

    LIBS += $$_PRO_FILE_PWD_/../shared//WindowsPackage/$$CONFIG_PATH/portaudio_x64.lib

    LIBS += $$_PRO_FILE_PWD_/../shared/WindowsPackage/$$CONFIG_PATH/opus.lib
    LIBS += $$_PRO_FILE_PWD_/../shared/WindowsPackage/$$CONFIG_PATH/celt.lib
    LIBS += $$_PRO_FILE_PWD_/../shared/WindowsPackage/$$CONFIG_PATH/silk_common.lib
    LIBS += $$_PRO_FILE_PWD_/../shared/WindowsPackage/$$CONFIG_PATH/silk_fixed.lib
    LIBS += $$_PRO_FILE_PWD_/../shared/WindowsPackage/$$CONFIG_PATH/silk_float.lib
}
# Unix (Linux and OsX)
unix: {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
    LIBS += -lportaudio
    LIBS += -lopus
}

DEFINES += AUDIO_USE_FLOAT

SOURCES += main.cpp\
    networkmgr.cpp \
    mainwindow.cpp \
    opcodemgr.cpp \
    widgetloginform.cpp \
    widgetcontactslist.cpp \
    widgetchatwindow.cpp \
    widgetchattab.cpp \
    encodedsample.cpp \
    audioencoder.cpp \
    audiomanager.cpp \
    audiosample.cpp \
    audiosocket.cpp \
    audiostream.cpp \
    packet.cpp \
    widgetaddcontactwindow.cpp \
    clientmgr.cpp \
    widgetchattabaddwindow.cpp

HEADERS  += \
    networkmgr.h \
    mainwindow.h \
    packet.hpp \
    opcodemgr.h \
    widgetloginform.h \
    widgetcontactslist.h \
    widgetchatwindow.h \
    widgetchattab.h \
    encodedsample.h \
    qsynchronizedqueue.h \
    audioencoder.h \
    audiomanager.h \
    audiosample.h \
    audiosocket.h \
    audiostream.h \
    widgetaddcontactwindow.h \
    clientmgr.h \
    widgetchattabaddwindow.h

FORMS += \
    widgetcontactslist.ui \
    widgetchatwindow.ui \
    widgetchattab.ui \
    widgetaddcontactwindow.ui \
    widgetchattabaddwindow.ui
