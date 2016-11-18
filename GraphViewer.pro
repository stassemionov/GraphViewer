#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T19:05:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    source/imageplace.cpp

HEADERS  += mainwindow.h \
    include/imageplace.h

FORMS    += mainwindow.ui
