#include <QCoreApplication>
#include "systemdev.h"

#ifndef Q_OS_WIN
#include "eventdispatcher_libev.h"
#endif

int main(int argc, char *argv[])
{
#ifndef Q_OS_WIN
    QCoreApplication::setEventDispatcher(new EventDispatcherLibEv());
#endif
    QCoreApplication a(argc, argv);
    SystemDev::getObject()->systemInit();
    return a.exec();
}
