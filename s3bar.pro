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
    panelentry.cpp \
    dialogkeys.cpp \
    lister.cpp

HEADERS  += mainwindow.h \
    panel.h \
    panelentry.h \
    dialogkeys.h \
    lister.h

FORMS    += mainwindow.ui \
    dialogkeys.ui

LIBS += -ls3 #-L$$PWD/libs3/build/lib -L/usr/lib/i386-linux-gnu -lcurl -lxml2

INCLUDEPATH += #$$PWD/libs3/inc -I/usr/include/libxml2
