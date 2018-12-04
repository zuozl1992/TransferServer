#include "serverdatahandle.h"
#include "networkpotocol.h"
#include "configure.h"
#include "control.h"
#include "infoOutput.h"

ServerDataHandle::ServerDataHandle(QObject *parent) : QObject(parent)
{
    connect(NetWorkPotocol::getObject(),SIGNAL(readPacket(QJsonObject)),
            this,SLOT(readPacketSlot(QJsonObject)));
}

ServerDataHandle *ServerDataHandle::getObject()
{
    static ServerDataHandle sdh;
    return &sdh;
}

void ServerDataHandle::readPacketSlot(const QJsonObject &data)
{
    if(data.value("deviceID").isUndefined())
        return;
    if(!data.value("deviceID").isDouble())
        return;
    if(data.value("deviceID").toInt() != Configure::getObject()->getDeviceID())
        return;
    if(data.value("type").isUndefined())
        return;
    if(!data.value("type").isString())
        return;
    QString type = data.value("type").toString();
    if(type == "controlCmd")
        handleControlCmdData(data);
}

void ServerDataHandle::handleControlCmdData(const QJsonObject &data)
{
    if(data.value("cmdType").isUndefined())
        return;
    if(!data.value("cmdType").isString())
        return;
    QString cmdType = data.value("cmdType").toString();
    if(cmdType == "setNode1Light")
    {
        if((!data.value("i").isDouble()) ||
                (!data.value("l").isDouble()))
            return;
        Control::getObject()->setNode1Light(data.value("i").toInt(),
                                            data.value("l").toInt());
    }
    else if(cmdType == "setNode1RGBLED")
    {
        if((!data.value("g").isDouble()) ||
                (!data.value("r").isDouble()) ||
                (!data.value("b").isDouble()))
            return;
        Control::getObject()->setNode1RGBLED(data.value("g").toInt(),
                                             data.value("r").toInt(),
                                             data.value("b").toInt());
    }
    else if(cmdType == "setNode5AlarmLight")
    {
        if(!data.value("sw").isBool())
            return;
        Control::getObject()->setNode5AlarmLight(data.value("sw").toBool());
    }
    else if(cmdType == "setNode5AlarmBuzzer")
    {
        if(!data.value("sw").isBool())
            return;
        Control::getObject()->setNode5AlarmBuzzer(data.value("sw").toBool());
    }
    else if(cmdType == "setNode6SteeringGear")
    {
        if(!data.value("sw").isBool())
            return;
        Control::getObject()->setNode6SteeringGear(data.value("sw").toBool());
    }
    else if(cmdType == "setNode7Fun")
    {
        if(!data.value("sw").isBool())
            return;
        Control::getObject()->setNode7Fun(data.value("sw").toBool());
    }
    else if(cmdType == "setNode7Motor")
    {
        if((!data.value("dir").isDouble()) ||
                (!data.value("len").isDouble()))
            return;
        Control::getObject()->setNode7Motor(data.value("dir").toInt(),
                                            data.value("len").toInt());
    }
    else if(cmdType == "setNode7Rec")
    {
        if((!data.value("id").isDouble()) ||
                (!data.value("sw").isBool()))
            return;
        Control::getObject()->setNode7Rec(data.value("id").toInt(),
                                            data.value("sw").toBool());
    }
    else if(cmdType == "setNode8Lock")
    {
        if(!data.value("sw").isBool())
            return;
        Control::getObject()->setNode8Lock(data.value("sw").toBool());
    }
}
