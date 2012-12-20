#-------------------------------------------------
#
# Project created by QtCreator 2012-11-30T10:47:21
#
#-------------------------------------------------

QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    upload.cpp \
    core.cpp \
    tools.cpp \
    protocol.cpp \
    transfermessage.cpp \
    uilibrary.cpp \
    media.cpp

HEADERS  += mainwindow.h \
    upload.h \
    core.h \
    tools.h \
    protocol.h \
    exception.h \
    transfermessage.h \
    uilibrary.h \
    media.h

FORMS    += mainwindow.ui \
    upload.ui \
    uilibrary.ui

OTHER_FILES +=

RESOURCES += \
    src.qrc
