#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T19:05:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphViewer
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES +=\
    source/main.cpp \
    source/mainwindow.cpp \
    source/viewer.cpp \
    source/dialogs.cpp \
    source/points.cpp \
    source/service.cpp

HEADERS  += \
    include/mainwindow.h \
    include/viewer.h \
    include/dialogs.h \
    include/points.h \
    include/service.h

FORMS    +=

RESOURCES += \
    resources.qrc
