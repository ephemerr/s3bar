#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T11:29:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = s3bar
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    panel.cpp \
    listservice.cpp \
    panelentry.cpp

HEADERS  += mainwindow.h \
    panel.h \
    listservice.h \
    panelentry.h

FORMS    += mainwindow.ui
