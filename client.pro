#-------------------------------------------------
#
# Project created by QtCreator 2012-11-30T10:47:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    upload.cpp \
    core.cpp \
    tools.cpp

HEADERS  += mainwindow.h \
    upload.h \
    core.h \
    tools.h

FORMS    += mainwindow.ui \
    upload.ui
