#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T13:01:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skypy
TEMPLATE = app

LIBS += -L../../lib/ -lshared
INCLUDEPATH += ../shared


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h
