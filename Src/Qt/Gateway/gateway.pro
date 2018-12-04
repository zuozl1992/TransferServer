#-------------------------------------------------
#
# Project created by QtCreator 2018-07-18T10:51:22
#
#-------------------------------------------------

QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(SerialPort/serialport.pri)
include(DataHandle/datahandle.pri)
include(Control/control.pri)
include(NetWork/network.pri)
include(StatusRecord/statusrecord.pri)
include(encryption/encryption.pri)

SOURCES += \
        main.cpp \
    maincontrol.cpp

DISTFILES += \
    run.sh

HEADERS += \
    maincontrol.h
