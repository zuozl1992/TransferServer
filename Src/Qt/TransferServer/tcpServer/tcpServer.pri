INCLUDEPATH     += $$PWD

QT              += network

HEADERS += \
        $$PWD/tcpserver.h \
        $$PWD/tcpsocket.h \
	$$PWD/threadhandle.h \
        $$PWD/tcpserverdev.h \
        $$PWD/tcpsocketdev.h \
        $$PWD/threadhandledev.h
SOURCES += \
        $$PWD/tcpserver.cpp \
        $$PWD/tcpsocket.cpp \
	$$PWD/threadhandle.cpp \
        $$PWD/tcpserverdev.cpp \
        $$PWD/tcpsocketdev.cpp \
        $$PWD/threadhandledev.cpp

