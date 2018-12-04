#-------------------------------------------------
#
# Project created by QtCreator 2018-09-25T15:19:05
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = AdminClient
TEMPLATE = app

include(encryption/encryption.pri)
include(network/network.pri)
include(StatusRecord/statusrecord.pri)

SOURCES += \
        main.cpp \
        widget.cpp

HEADERS += \
        widget.h

FORMS += \
        widget.ui

RC_FILE += \
        version.rc

RESOURCES += \
    images.qrc
