#-------------------------------------------------
#
# Project created by QtCreator 2017-08-07 14:24:31
#
#-------------------------------------------------

QT          += core


TARGET      = TransferServer
TEMPLATE    = app

QT          -= gui
CONFIG      += console
CONFIG      -= app_bundle
CONFIG      += C++11

SOURCES += main.cpp \
    systemdev.cpp

HEADERS  += \ 
    systemdev.h

unix:include($$PWD/eventdispatcher_libev/eventdispatcher_libev.pri)
include($$PWD/tcpServer/tcpServer.pri)
include($$PWD/encryption/encryption.pri)
include($$PWD/statusRecord/statusRecord.pri)
include($$PWD/mysqlDataManagement/mysqlDataManagement.pri)

