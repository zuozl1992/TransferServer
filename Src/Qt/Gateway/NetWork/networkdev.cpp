#include "networkdev.h"
#include "infoOutput.h"

NetWorkDev::NetWorkDev(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;

    connect(socket,SIGNAL(readyRead()),
            this,SLOT(socketReadyReadSlot()));
    connect(socket,SIGNAL(connected()),
            this,SLOT(socketConnectedSlot()));
    connect(socket,SIGNAL(disconnected()),
            this,SLOT(socketDisconnectedSlot()));

    InfoOutput::putOutInfo("NetWorkDev init");
}

NetWorkDev *NetWorkDev::getObject()
{
    static NetWorkDev nd;
    return &nd;
}

void NetWorkDev::openNet(const QString &ipAddr, quint16 port)
{
    if(!socket)
        return;
    socket->close();
    socket->connectToHost(ipAddr,port);
    InfoOutput::putOutInfo(tr("NetWorkDev open %1:%2")
                           .arg(ipAddr)
                           .arg(port));
}

bool NetWorkDev::getNetStat()
{
    return isConnected;
}

void NetWorkDev::closeNet()
{
    socket->close();
    isConnected = false;
}

bool NetWorkDev::sendData(const QByteArray &data)
{
    if(!socket)
        return false;
    if(!getNetStat())
        return false;
    qint64 re = socket->write(data);
    return re==-1 ? false : true;
}

void NetWorkDev::socketReadyReadSlot()
{
    QByteArray data;
    data.append(socket->readAll());
    emit readData(data);
}

void NetWorkDev::socketConnectedSlot()
{
    InfoOutput::putOutInfo("NetWorkDev open successful");
    isConnected = true;
}

void NetWorkDev::socketDisconnectedSlot()
{
    isConnected = false;
}
