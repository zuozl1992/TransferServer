#include <QCoreApplication>
#include "maincontrol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainControl::getObject();

    return a.exec();
}
