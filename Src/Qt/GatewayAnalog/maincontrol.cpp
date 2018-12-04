#include "maincontrol.h"
#include "networkdev.h"
#include "networkpotocol.h"
#include "infoOutput.h"
#include "configure.h"
#include "serverdatahandle.h"
#include "datacache.h"

MainControl::MainControl(QObject *parent) : QObject(parent)
{
    InfoOutput::getobject();
    Configure::getObject();

    NetWorkDev::getObject();
    NetWorkPotocol::getObject();

    DataCache::getObject();

    ServerDataHandle::getObject();

    updateTimer = new QTimer;
    connect(updateTimer,SIGNAL(timeout()),
            this,SLOT(updateTimerTimeoutSlot()));
    updateTimer->start(5000);
    updateTimerTimeoutSlot();
}

MainControl *MainControl::getObject()
{
    static MainControl ct;
    return &ct;
}

void MainControl::updateTimerTimeoutSlot()
{
    if(!NetWorkDev::getObject()->getNetStat())
        NetWorkDev::getObject()->openNet(
                    Configure::getObject()->getNetWorkIpAddr(),
                    Configure::getObject()->getNetWorkPort());
}
