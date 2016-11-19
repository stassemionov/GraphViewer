#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T19:05:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphViewer
TEMPLATE = app


SOURCES +=\
    source/main.cpp \
    source/mainwindow.cpp \
    source/viewer.cpp

HEADERS  += \
    include/mainwindow.h \
    include/viewer.h

FORMS    += mainwindow.ui
