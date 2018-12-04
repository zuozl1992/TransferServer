#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "network.h"

NetWork::NetWork(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(readyReadSlot()));
    connect(socket,SIGNAL(connected()),
            this,SIGNAL(clientConnected()));
    connect(socket,SIGNAL(disconnected()),
            this,SIGNAL(clientDisconnected()));
    readBuffer.clear();
}

NetWork *NetWork::getObject()
{
    static NetWork nw;
    return &nw;
}

void NetWork::connectToServer(QString ip, quint16 port)
{
    if(getSocketState())
    {
        disconnect(socket,0,0,0);
        socket->abort();
    }
    socket->connectToHost(ip,port,QIODevice::ReadWrite);
}

void NetWork::login(QString userName, QString passWord)
{
    QJsonObject obj;
    obj.insert("type","login");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",userName);
    obj.insert("passWord",passWord);
    sendJson(obj);
}

void NetWork::logout()
{
    QJsonObject obj;
    obj.insert("type","logout");
    obj.insert("requestIndex",requestIndex++);
    sendJson(obj);
}

void NetWork::adminLogin(QString userName, QString passWord)
{
    QJsonObject obj;
    obj.insert("type","adminLogin");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",userName);
    obj.insert("passWord",passWord);
    sendJson(obj);
}

void NetWork::getSchoolList()
{
    QJsonObject obj;
    obj.insert("type","getSchoolList");
    obj.insert("requestIndex",requestIndex++);
    sendJson(obj);
}

void NetWork::getMajorList(const QString &schoolName)
{
    QJsonObject obj;
    obj.insert("type","getMajorList");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    sendJson(obj);
}

void NetWork::getClassList(const QString &schoolName, const QString &majorName)
{
    QJsonObject obj;
    obj.insert("type","getClassList");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    sendJson(obj);
}

void NetWork::getUserList(const QString &schoolName, const QString &majorName, const QString &className)
{
    QJsonObject obj;
    obj.insert("type","getUserList3");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    obj.insert("className",className);
    sendJson(obj);
}

void NetWork::getUserList(const QString &schoolName, const QString &majorName)
{
    QJsonObject obj;
    obj.insert("type","getUserList2");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    sendJson(obj);
}

void NetWork::getUserList(const QString &schoolName)
{
    QJsonObject obj;
    obj.insert("type","getUserList1");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    sendJson(obj);
}

void NetWork::getUserList()
{
    QJsonObject obj;
    obj.insert("type","getErrorUserList");
    obj.insert("requestIndex",requestIndex++);
    sendJson(obj);
}

void NetWork::getAdminList()
{
    QJsonObject obj;
    obj.insert("type","getAdminList");
    obj.insert("requestIndex",requestIndex++);
    sendJson(obj);
}

void NetWork::getUserInfo(const QString &userName)
{
    QJsonObject obj;
    obj.insert("type","getUserInfo");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",userName);
    sendJson(obj);
}

void NetWork::getMyInfo()
{
    QJsonObject obj;
    obj.insert("type","getMyInfo");
    obj.insert("requestIndex",requestIndex++);
    sendJson(obj);
}

void NetWork::addNewClass(const QString &schoolName, const QString &majorName, const QString &className)
{
    QJsonObject obj;
    obj.insert("type","addNewClass");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    obj.insert("name",className);
    sendJson(obj);
}

void NetWork::addNewMajor(const QString &schoolName, const QString &majorName)
{
    QJsonObject obj;
    obj.insert("type","addNewMajor");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("name",majorName);
    sendJson(obj);
}

void NetWork::addNewSchool(const QString &schoolName)
{
    QJsonObject obj;
    obj.insert("type","addNewSchool");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("name",schoolName);
    sendJson(obj);
}

void NetWork::addNewDevice(const QString &cDevicePasswd)
{
    QJsonObject obj;
    obj.insert("type","addNewDevice");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("passWord",cDevicePasswd);
    sendJson(obj);
}

void NetWork::addNewAdmin(const QString &cUserName, const QString &cPasswd)
{
    QJsonObject obj;
    obj.insert("type","adminRegister");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("passWord",cPasswd);
    sendJson(obj);
}

void NetWork::updateClass(const QString &schoolName, const QString &majorName, const QString &oldName, const QString &newName)
{
    QJsonObject obj;
    obj.insert("type","updateClass");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    obj.insert("oldName",oldName);
    obj.insert("newName",newName);
    sendJson(obj);
}

void NetWork::updateMajor(const QString &schoolName, const QString &oldName, const QString &newName)
{
    QJsonObject obj;
    obj.insert("type","updateMajor");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("oldName",oldName);
    obj.insert("newName",newName);
    sendJson(obj);
}

void NetWork::updateSchool(const QString &oldName, const QString &newName)
{
    QJsonObject obj;
    obj.insert("type","updateSchool");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("oldName",oldName);
    obj.insert("newName",newName);
    sendJson(obj);
}

void NetWork::updateUserInfo(const QString &cUserName, const QString &cName, const QString &cSchool, const QString &cMajor, const QString &cClass, const QString &cSchoolNumber, const QString &cPost, const QString &cContactInfo)
{
    QJsonObject obj;
    obj.insert("type","updateUserInfo3");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("name",cName);
    obj.insert("school",cSchool);
    obj.insert("major",cMajor);
    obj.insert("class",cClass);
    obj.insert("schoolNumber",cSchoolNumber);
    obj.insert("post",cPost);
    obj.insert("contactInfo",cContactInfo);
    sendJson(obj);
}

void NetWork::updateUserInfo(const QString &cUserName, const QString &cName, const QString &cSchoolNumber, const QString &cPost, const QString &cContactInfo)
{
    QJsonObject obj;
    obj.insert("type","updateUserInfo4");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("name",cName);
    obj.insert("schoolNumber",cSchoolNumber);
    obj.insert("post",cPost);
    obj.insert("contactInfo",cContactInfo);
    sendJson(obj);
}

void NetWork::updateMyInfo(const QString &cUserName, const QString &cName, const QString &cSchool, const QString &cMajor, const QString &cClass, const QString &cSchoolNumber, const QString &cPost, const QString &cContactInfo)
{
    QJsonObject obj;
    obj.insert("type","updateUserInfo1");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("name",cName);
    obj.insert("school",cSchool);
    obj.insert("major",cMajor);
    obj.insert("class",cClass);
    obj.insert("schoolNumber",cSchoolNumber);
    obj.insert("post",cPost);
    obj.insert("contactInfo",cContactInfo);
    sendJson(obj);
}

void NetWork::updateMyInfo(const QString &cUserName, const QString &cName, const QString &cSchoolNumber, const QString &cPost, const QString &cContactInfo)
{
    QJsonObject obj;
    obj.insert("type","updateUserInfo2");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("name",cName);
    obj.insert("schoolNumber",cSchoolNumber);
    obj.insert("post",cPost);
    obj.insert("contactInfo",cContactInfo);
    sendJson(obj);
}

void NetWork::updateAdminPassword(const QString &cUserName, const QString &cPasswd)
{
    QJsonObject obj;
    obj.insert("type","adminUpdatePassWord");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("passWord",cPasswd);
    sendJson(obj);
}

void NetWork::updatePassWord1(const QString &cUserName, const QString &coldPasswd, const QString &cPasswd)
{
    QJsonObject obj;
    obj.insert("type","updatePassWord1");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("oldPassWord",coldPasswd);
    obj.insert("passWord",cPasswd);
    sendJson(obj);
}

void NetWork::updatePassWord2(const QString &cUserName, const QString &cPasswd)
{
    QJsonObject obj;
    obj.insert("type","updatePassWord2");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",cUserName);
    obj.insert("passWord",cPasswd);
    sendJson(obj);
}

void NetWork::setClassStateOfRegistration(const QString &schoolName, const QString &majorName, const QString &className, bool state)
{
    QJsonObject obj;
    obj.insert("type","setClassStateOfRegistration");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    obj.insert("className",className);
    obj.insert("state",state);
    sendJson(obj);
}

void NetWork::deleteClass(const QString &schoolName, const QString &majorName, const QString &className)
{
    QJsonObject obj;
    obj.insert("type","deleteClass");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("majorName",majorName);
    obj.insert("name",className);
    sendJson(obj);
}

void NetWork::deleteMajor(const QString &schoolName, const QString &majorName)
{
    QJsonObject obj;
    obj.insert("type","deleteMajor");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("schoolName",schoolName);
    obj.insert("name",majorName);
    sendJson(obj);
}

void NetWork::deleteSchool(const QString &schoolName)
{
    QJsonObject obj;
    obj.insert("type","deleteSchool");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("name",schoolName);
    sendJson(obj);
}

void NetWork::deleteUser(const QString &userName)
{
    QJsonObject obj;
    obj.insert("type","deleteUser");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",userName);
    sendJson(obj);
}

void NetWork::getSensingData(int id, const QString &password)
{
    QJsonObject obj;
    obj.insert("type","requestData");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("deviceID",id);
    obj.insert("passWord",password);
    sendJson(obj);
}

void NetWork::controlCmd(QJsonObject obj)
{
    obj.insert("requestIndex",requestIndex++);
    sendJson(obj);
}

void NetWork::addNewUser(const QString &userName, const QString &password)
{
    QJsonObject obj;
    obj.insert("type","register");
    obj.insert("requestIndex",requestIndex++);
    obj.insert("userName",userName);
    obj.insert("passWord",password);
    sendJson(obj);
}

void NetWork::readyReadSlot()
{
    QByteArray data = socket->readAll();
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
                handleData(readBuffer);
                readBuffer.clear();
                braNum = 0;
            }
            continue;
        }
        else
        {
            readBuffer.append(data.at(i));
            if(data.at(i) != ' ' && data.at(i) != '\n'
                   && data.at(i) != '\t' && data.at(i) != 10)
                lastChar = data.at(i);
            continue;
        }
    }
}

void NetWork::sendJson(QJsonObject obj)
{
    if(!getSocketState())
        return;
    obj.insert("key",key);
    obj.insert("adminKey",adminKey);
    socket->write(QJsonDocument(obj).toJson());
    emit newDataSend();
}

bool NetWork::getSocketState()
{
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        return true;
    }
    return false;
}

void NetWork::handleData(QByteArray data)
{
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    QString type = obj.value("type").toString();
    if(type == "error")
    {
        QString data = obj.value("data").toString();
        emit reErrorInfo(data);
    }
    else if(type == "dropLine")
    {
        emit dropLine();
    }
    else if(type == "reAdminLogin")
    {
        bool re = obj.value("re").toBool();
        if(re)
        {
            key = obj.value("key").toString();
            adminKey = obj.value("adminKey").toString();
        }
        emit reAdminLogin(re);
    }
    else if(type == "reLogin")
    {
        bool re = obj.value("re").toBool();
        if(re)
        {
            key = obj.value("key").toString();
        }
        emit reLogin(re);
    }
    else if(type == "reGetSchoolList")
    {
        emit reGetSchoolList(obj.value("re").toArray());
    }
    else if(type == "reGetMajorList")
    {
        emit reGetMajorList(obj.value("re").toArray());
    }
    else if(type == "reGetClassList")
    {
        emit reGetClassList(obj.value("re").toArray());
    }
    else if(type == "reGetUserList")
    {
        emit reGetUserList(obj.value("re").toArray());
    }
    else if(type == "reGetUserInfo")
    {
        emit reGetUserInfo(obj.value("re").toObject());
    }
    else if(type == "reGetMyInfo")
    {
        emit reGetMyInfo(obj.value("re").toObject());
    }
    else if(type == "reLogout")
    {
        emit dropLine();
    }
    else if(type == "reGetAdminList")
    {
        emit reGetAdminList(obj.value("re").toArray());
    }
    else if(type == "reAddNewDevice")
    {
        bool re = obj.value("re").toBool();
        if(re)
            emit reNewDeviceID(obj.value("id").toInt());
        else
            emit reBool(false);
    }
    else if(type == "reRequestData")
    {
        if(obj.value("re").toBool())
            emit showData(obj.value("data").toObject());
    }
    else
    {
        bool ok = obj.value("re").toBool();
        emit reBool(ok);
    }
}
