#include "serialportpotocol.h"
#include "serialportdev.h"
#include "infoOutput.h"
#include <QDebug>

SerialPortPotocol::SerialPortPotocol(QObject *parent) : QObject(parent)
{
    connect(SerialPortDev::getObject(),SIGNAL(readData(QByteArray)),
            this,SLOT(readDataSlot(QByteArray)));
    InfoOutput::putOutInfo("SerialPortPotocol init");
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            nodeAddr[j][i] = 0xFF;
        }
    }
}

SerialPortPotocol *SerialPortPotocol::getObject()
{
    static SerialPortPotocol sp;
    return &sp;
}

bool SerialPortPotocol::sendPacket(SendData data)
{
    QByteArray sendData;
    data.head[0] = nodeAddr[data.nodeID-1][0];
    data.head[1] = nodeAddr[data.nodeID-1][1];
    sendData.append((char *)data.head,2);
    sendData.append((qint8)data.reserved);
    sendData.append((char *)data.addr,2);
    sendData.append((qint8)data.len);
    sendData.append((qint8)data.nodeID);
    sendData.append((qint8)data.devID);
    sendData.append((char *)data.cmd,data.len - 9);
    sendData.append((qint8)data.foot);
    return SerialPortDev::getObject()->sendData(sendData);
}

void SerialPortPotocol::readDataSlot(const QByteArray &data)
{
    for(quint16 i = 0; i < data.size(); i++)
    {
        //帧头
        if((quint8)data[i] == 0xEF && len > 0)
        {
            if(readBuffer[len-1] == 0xEF)
            {
                readBuffer[0] = 0xEF;
                readBuffer[1] = 0xEF;
                len = 2;
                continue;
            }
        }
        //帧尾
        if(len > 5 && len + 1 == readBuffer[5] && (quint8)data[i] == 0xFF)
        {
            readBuffer[len++] = data[i];
            //接收到一帧数据
            handleData();
            len = 0;
            continue;
        }
        //普通数据
        readBuffer[len++] = data[i];
    }
}

void SerialPortPotocol::handleData()
{
    ReadData data;
    data.head[0] = readBuffer[0];
    data.head[1] = readBuffer[1];
    data.reserved = readBuffer[2];
    data.addr[0] = readBuffer[3];
    data.addr[1] = readBuffer[4];
    data.len = readBuffer[5];
    data.nodeID = readBuffer[6];
    data.devID = readBuffer[7];
    for(quint16 i = 8; i < data.len - 1; i++)
        data.data[i - 8] = readBuffer[i];
    data.foot = readBuffer[len - 1];
    emit readPacket(data);
}

void SerialPortPotocol::updateNodeAddr(quint8 nodeID, quint8 addr1, quint8 addr2)
{
    nodeAddr[nodeID-1][0] = addr1;
    nodeAddr[nodeID-1][2] = addr2;
}
