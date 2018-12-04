QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

include(DataHandle/datahandle.pri)
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
