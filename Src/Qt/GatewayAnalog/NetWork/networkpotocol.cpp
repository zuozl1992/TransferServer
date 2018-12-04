#include <QJsonDocument>
#include <QDebug>
#include "networkpotocol.h"
#include "networkdev.h"
#include "infoOutput.h"
#include "encryption.h"
#include "configure.h"

NetWorkPotocol::NetWorkPotocol(QObject *parent) : QObject(parent)
{
    connect(NetWorkDev::getObject(),SIGNAL(readData(QByteArray)),
            this,SLOT(readDataSlot(QByteArray)));

    heartBeatTimer = new QTimer;
    connect(heartBeatTimer,SIGNAL(timeout()),
            this,SLOT(heartBeatTimerTimeoutSlot()));
    heartBeatTimer->start(15000);

    InfoOutput::putOutInfo("NetWorkPotocol init");
}

NetWorkPotocol *NetWorkPotocol::getObject()
{
    static NetWorkPotocol np;
    return &np;
}

bool NetWorkPotocol::sendPacket(const QJsonObject &data)
{
    QJsonObject newData = data;
    newData.insert("key",key);
    bool ok = NetWorkDev::getObject()->sendData(QJsonDocument(newData).toJson());
    return ok;
}

void NetWorkPotocol::readDataSlot(const QByteArray &data)
{
    for(qint64 i = 0; i < data.length(); i++)
    {
        if(readBuffer.length() > 0 && data.at(i) == '{'
                && lastChar != ':' && lastChar != '['
                && lastChar != ',')
        {
            //接收到开头，之前的数据不完整丢弃
            readBuffer.clear();
            readBuffer.append(data.at(i));
            lastChar = data.at(i);
            braNum = 1;
            continue;
        }
        if(data.at(i) == '{')
        {
            braNum++;
            readBuffer.append(data.at(i));
            lastChar = data.at(i);
            continue;
        }
        else if(data.at(i) == '}')
        {
            braNum--;
            readBuffer.append(data.at(i));
            lastChar = data.at(i);
            if(braNum == 0)
            {
                //一条完整数据
                cacheReadBuffer();
                readBuffer.clear();
                braNum = 0;
            }
            continue;
        }
        else
        {
            readBuffer.append(data.at(i));
            if(data.at(i) != ' ' && data.at(i) != '\n' &&
                    data.at(i) != '\t' && data.at(i) != 0x0a)
                lastChar = data.at(i);
            continue;
        }
    }
}

void NetWorkPotocol::heartBeatTimerTimeoutSlot()
{
    if(!isReceivedHeart)
    {
        if(NetWorkDev::getObject()->getNetStat())
            NetWorkDev::getObject()->closeNet();
    }
    else
    {
        isReceivedHeart = false;
    }
}

void NetWorkPotocol::cacheReadBuffer()
{
    QJsonObject obj = QJsonDocument::fromJson(readBuffer).object();
    QString type = obj.value("type").toString();
    if(type == "heartBeat")
    {
        isReceivedHeart = true;
    }
    else if(type == "error")
    {
        QJsonObject log;
        log.insert("type","deviceLogin");
        log.insert("requestIndex",301);
        log.insert("id",Configure::getObject()->getDeviceID());
        log.insert("pass",Encryption::md5( Configure::getObject()->getDevicePass()));
        sendPacket(log);
    }
    else if(type == "reDeviceLogin")
    {
        bool re = obj.value("re").toBool();
        if(re)
            key = obj.value("key").toString();
    }
    else
        emit readPacket(obj);
}
