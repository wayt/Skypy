#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T13:01:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skypy
TEMPLATE = app

SOURCES += main.cpp\
    networkmgr.cpp \
    mainwindow.cpp \
    opcodemgr.cpp \
    widgetloginform.cpp \
    widgetcontactslist.cpp \
    widgetchatwindow.cpp \
    widgetchattab.cpp

HEADERS  += \
    networkmgr.h \
    mainwindow.h \
    packet.hpp \
    opcodemgr.h \
    widgetloginform.h \
    widgetcontactslist.h \
    Singleton.hpp \
    widgetchatwindow.h \
    widgetchattab.h

FORMS += \
    widgetcontactslist.ui \
    widgetchatwindow.ui \
    widgetchattab.ui
