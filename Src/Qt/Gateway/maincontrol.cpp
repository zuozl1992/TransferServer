#include "maincontrol.h"
#include "serialportdev.h"
#include "serialportpotocol.h"
#include "datacache.h"
#include "control.h"
#include "networkdev.h"
#include "networkpotocol.h"
#include "infoOutput.h"
#include "configure.h"
#include "serverdatahandle.h"

MainControl::MainControl(QObject *parent) : QObject(parent)
{
    InfoOutput::getobject();
    Configure::getObject();

    SerialPortDev::getObject();
    SerialPortPotocol::getObject();

    DataCache::getObject();
    Control::getObject();

    NetWorkDev::getObject();
    NetWorkPotocol::getObject();

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
    if(!SerialPortDev::getObject()->getPortStat())
        SerialPortDev::getObject()->openPort(
                    Configure::getObject()->getSerialPortName());
    if(!NetWorkDev::getObject()->getNetStat())
        NetWorkDev::getObject()->openNet(
                    Configure::getObject()->getNetWorkIpAddr(),
                    Configure::getObject()->getNetWorkPort());
}
