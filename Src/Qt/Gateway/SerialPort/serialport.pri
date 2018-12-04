INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += serialport

HEADERS += \
    $$PWD/serialportdev.h \
    $$PWD/serialportpotocol.h

SOURCES += \
    $$PWD/serialportdev.cpp \
    $$PWD/serialportpotocol.cpp
