#include "tcpsocketdev.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonObject>
#include "infoOutput.h"
#include "mysqldev.h"
#include "encryption.h"
#include "journal.h"

TcpSocketDev::TcpSocketDev(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    deviceKey = Encryption::generateUserIdentity("Unspecified");
    deviceID = 0;
    devicePass.clear();
    readBuffer.clear();
    //绑定接受数据信号
    connect(this,SIGNAL(readyRead()),
            this,SLOT(readDataSlot()));
    //绑定断开信号
    dis = connect(this,SIGNAL(disconnected()),
                  this,SLOT(disconnectedSlot()));
    //绑定QFutureWatcher处理信号
    connect(&rawDataWatcher,SIGNAL(finished()),
            this,SLOT(startNext()));
    connect(&rawDataWatcher,SIGNAL(canceled()),
            this,SLOT(startNext()));
    connect(this,SIGNAL(oneFullDataRead(QByteArray)),
            this,SLOT(oneFullDataReadSlot(QByteArray)));
    Journal::recordData(2,tr("Device client connected,socketId:%1,ip:%2")
                        .arg(socketID)
                        .arg(this->peerAddress().toString()));
}

TcpSocketDev::~TcpSocketDev()
{
}

void TcpSocketDev::sentData(const QJsonObject &obj, const int id)
{
    if(id == socketID)
    {
        QJsonDocument doc(obj);
        QByteArray bytedata = doc.toJson();
        this->write(bytedata);
        this->flush();
    }
    else if( id == -1 )
    {
        QJsonDocument doc(obj);
        QByteArray bytedata = doc.toJson();
        this->write(bytedata);
        this->flush();
    }
}

void TcpSocketDev::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocketDev::readDataSlot()
{
    rawDataQueue.append(this->readAll());
    if(rawDataQueue.length() > 10)
    {
        QString info = tr("The device server suffered an attack,ip:%1,port:%2.")
                .arg(this->peerAddress().toString())
                .arg(this->peerPort());
        this->abort();
        if(deviceID == 0)
            info.append("The attacker is not logged in.");
        else
            info.append(tr("The attacker is logged in,device id:%1.").arg(deviceID));
        InfoOutput::putOutWarningInfo(info);
        Journal::recordData(1,info);
    }
    if (!rawDataWatcher.isRunning())//放到异步线程中处理。
    {
        rawDataWatcher.setFuture(QtConcurrent::run(this,&TcpSocketDev::handleData,rawDataQueue.dequeue()));
    }
}

void TcpSocketDev::disconnectedSlot()
{
    Journal::recordData(2,tr("Client disconnected,socketId:%1").arg(socketID));
    emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
    this->deleteLater();
}

void TcpSocketDev::handleData(const QByteArray &data)
{
    QTime time;
    time.start();
    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
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
            lastChar = '{';
            continue;
        }
        else if(data.at(i) == '}')
        {
            braNum--;
            readBuffer.append(data.at(i));
            lastChar = '}';
            if(braNum == 0)
            {
                //一条完整数据
                emit oneFullDataRead(readBuffer);
                readBuffer.clear();
                braNum = 0;
            }
            continue;
        }
        else
        {
            readBuffer.append(data.at(i));
            if(data.at(i) != ' ' && data.at(i) != '\n'
               && data.at(i) != '\t' && data.at(i) != 0x0a)
                lastChar = data.at(i);
            continue;
        }
    }
}

void TcpSocketDev::startNext()
{
    if (!rawDataQueue.isEmpty())
    {
        rawDataWatcher.setFuture(QtConcurrent::run(this,&TcpSocketDev::handleData,rawDataQueue.dequeue()));
    }
}

void TcpSocketDev::sendData(const QJsonObject &jsonObj)
{
    QJsonDocument doc(jsonObj);
    QByteArray bytedata = doc.toJson();
    this->write(bytedata);
    this->flush();
}

void TcpSocketDev::reError()
{
    QJsonObject obj;
    obj.insert("type","error");
    sendData(obj);
}

void TcpSocketDev::reDeviceLogin(int index, bool re, const QString &key)
{
    QJsonObject obj;
    obj.insert("type","reDeviceLogin");
    obj.insert("requestIndex",index);
    obj.insert("re",re);
    if(re)
    {
        obj.insert("key",key);
    }
    sendData(obj);
}

bool TcpSocketDev::checkDeviceKey(const QJsonObject &obj)
{
    QString dataDevicekey = obj.value("key").toString();
    if(dataDevicekey != deviceKey)
    {
        reError();
        return false;
    }
    return true;
}

void TcpSocketDev::oneFullDataReadSlot(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();
    if(type == "deviceLogin")
    {
        int index = obj.value("requestIndex").toInt();
        int id = obj.value("id").toInt();
        QString pass = obj.value("pass").toString();
        bool ok = MySqlDev::getobject()->deviceCheck(id,pass);
        if(ok)
        {
            deviceKey = Encryption::generateUserIdentity(tr("%1").arg(id));
            deviceID = id;
            devicePass = pass;
        }
        reDeviceLogin(index,ok,deviceKey);
    }
    else if(type == "sensingData")
    {
        if(!checkDeviceKey(obj))
            return;
        sensingData = obj;
        sensingData.insert("key","");
    }
}

void TcpSocketDev::serverOneDeviceLoginSucceedSlot(int i, int deviceID)
{
    if( (socketID != i) && (this->deviceID == deviceID) )
    {
        QJsonObject obj;
        obj.insert("type","dropLine");
        sendData(obj);
        this->deviceKey = Encryption::generateUserIdentity("Unspecified");
        this->deviceID = 0;
        this->devicePass.clear();
        this->readBuffer.clear();
    }
}

void TcpSocketDev::requestSensingDataSlot(int sptr, int index, int id, const QString &pass)
{
    if(id != deviceID)
        return;
    if(pass == devicePass)
    {
        emit reRequestSensingData(sptr,index,true,sensingData);
    }
    else
    {
        emit reRequestSensingData(sptr,index,false,QJsonObject());
    }
}

void TcpSocketDev::controlCmdSlot(int sptr, const QJsonObject &cmd)
{
    int id = cmd.value("deviceID").toInt();
    if(id != deviceID)
        return;
    QString password = cmd.value("passWord").toString();
    int index = cmd.value("requestIndex").toInt();
    if(password == devicePass)
    {
        sendData(cmd);
        emit reControlCmd(sptr,index,true);
    }
    else
    {
        emit reControlCmd(sptr,index,false);
    }
}
