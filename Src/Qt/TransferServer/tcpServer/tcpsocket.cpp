#include "tcpsocket.h"
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

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    userKey = Encryption::generateUserIdentity("Unspecified");
    adminKey = Encryption::generateUserIdentity("Unspecified");
    userName.clear();
    adminName.clear();
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
    Journal::recordData(2,tr("Client connected,socketId:%1,ip:%2")
                        .arg(socketID)
                        .arg(this->peerAddress().toString()));
}

TcpSocket::~TcpSocket()
{
}


void TcpSocket::sentData(const QJsonObject &obj, const int id)
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

void TcpSocket::disConTcp(int i)
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

void TcpSocket::readDataSlot()
{
    //追加到队列中
    rawDataQueue.append(this->readAll());
    if(rawDataQueue.length() > 10)
    {
        QString info = tr("The server suffered an attack,ip:%1,port:%2.")
                .arg(this->peerAddress().toString())
                .arg(this->peerPort());
        this->abort();
        if(!adminName.isEmpty())
        {
            info.append(tr("Attacker has administrator privileges,adminName:%1.").arg(adminName));
        }
        else if(!userName.isEmpty())
        {
            info.append(tr("Attacker is a normal user,username:%1.").arg(userName));
        }
        else
        {
            info.append("The attacker is not logged in.");
        }
        InfoOutput::putOutWarningInfo(info);
        Journal::recordData(1,info);
    }
    //异步处理线程空闲则立即处理
    if (!rawDataWatcher.isRunning())
    {
        rawDataWatcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,rawDataQueue.dequeue()));
    }
}

void TcpSocket::disconnectedSlot()
{
    Journal::recordData(2,tr("Client disconnected,socketId:%1").arg(socketID));
    emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
    this->deleteLater();
}

void TcpSocket::handleData(const QByteArray &data)
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

void TcpSocket::startNext()
{
    if (!rawDataQueue.isEmpty())
    {
        rawDataWatcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,rawDataQueue.dequeue()));
    }
}

void TcpSocket::sendData(const QJsonObject &jsonObj)
{
    QJsonDocument doc(jsonObj);
    QByteArray bytedata = doc.toJson();
    this->write(bytedata);
    this->flush();
}

bool TcpSocket::checkAdminKey(const QJsonObject &obj)
{
    QString dataUserkey = obj.value("key").toString();
    QString dataAdminkey = obj.value("adminKey").toString();
    int index = obj.value("requestIndex").toInt();
    if((dataUserkey != userKey) || (dataAdminkey != adminKey))
    {
        reError("请重新登录",index);
        return false;
    }
    return true;
}

bool TcpSocket::checkUserKey(const QJsonObject &obj)
{
    QString dataUserkey = obj.value("key").toString();
    int index = obj.value("requestIndex").toInt();
    if(dataUserkey != userKey)
    {
        reError("请重新登录",index);
        return false;
    }
    return true;
}

void TcpSocket::reError(const QString &data, int index)
{
    QJsonObject obj;
    obj.insert("type","error");
    obj.insert("requestIndex",index);
    obj.insert("data",data);
    sendData(obj);
}

void TcpSocket::oneFullDataReadSlot(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();
    if(type == "login")
    {
        QString userName = obj.value("userName").toString();
        QString passWord = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->userLogin(userName,passWord);
        if(ok)
        {
            this->userName = userName;
            this->userKey = Encryption::generateUserIdentity(userName);
            this->isAcessToOperation = MySqlDev::getobject()->checkUserOperationJurisdiction(userName);
            emit oneUserLoginSucceed(socketID,this->userName);
        }
        int index = obj.value("requestIndex").toInt();
        reLogin(ok,userKey,index);
    }
    else if(type == "register")
    {
        QString userName = obj.value("userName").toString();
        QString passWord = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->addNewUser(userName,passWord,
                                                    this->peerAddress().toString());
        int index = obj.value("requestIndex").toInt();
        reRegister(ok,index);
    }
    else if(type == "adminLogin")
    {
        QString userName = obj.value("userName").toString();
        QString passWord = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->adminLogin(userName,passWord);
        if(ok)
        {
            this->adminName = userName;
            this->userKey = Encryption::generateUserIdentity(userName);
            this->adminKey = Encryption::generateUserIdentity(userName);
            emit oneAdminLoginSucceed(socketID,adminName);
        }
        int index = obj.value("requestIndex").toInt();
        reAdminLogin(ok,userKey,adminKey,index);
    }
    else if(type == "adminRegister")
    {
        if(!checkAdminKey(obj))
            return;
        QString userName = obj.value("userName").toString();
        QString passWord = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->addNewAdmin(userName,passWord);
        int index = obj.value("requestIndex").toInt();
        reAdminRegister(ok,index);
    }
    else if(type == "adminUpdatePassWord")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();;
        QString userName = obj.value("userName").toString();
        if(userName != adminName)
        {
            reBool("reAdminUpdatePassWord",false,index);
            Journal::recordData(1,tr("Admin update password failed,ipaddr:%1.").arg(this->peerAddress().toString()));
            return;
        }
        QString password = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->updateAdminPassword(userName,password);
        Journal::recordData(2,tr("Admin update password successful,admin userName:%1.").arg(adminName));
        reBool("reAdminUpdatePassWord",ok,index);
        reLogout(-1);
    }
    else if(type == "updatePassWord2")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();;
        QString userName = obj.value("username").toString();
        QString password = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->updateUserPassword(userName,password);
        reBool("reUpdatePassWord2",ok,index);
    }
    else if(type == "addNewSchool")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString name = obj.value("name").toString();
        bool ok = MySqlDev::getobject()->addNewSchool(name);
        reBool("reAddNewSchool",ok,index);
    }
    else if(type == "addNewMajor")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString name = obj.value("name").toString();
        bool ok = MySqlDev::getobject()->addNewMajor(schoolName,name);
        reBool("reAddNewMajor",ok,index);
    }
    else if(type == "addNewClass")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        QString name = obj.value("name").toString();
        bool ok = MySqlDev::getobject()->addNewClass(schoolName,majorName,name);
        reBool("reAddNewClass",ok,index);
    }
    else if(type == "addNewDevice")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString passWord = obj.value("passWord").toString();
        int id = MySqlDev::getobject()->addNewDevice(passWord);
        bool ok = (id == -1) ? false : true;
        reAddNewDevice(id,ok,index);
    }
    else if(type == "deleteSchool")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString name = obj.value("name").toString();
        bool ok = MySqlDev::getobject()->deleteSchool(name);
        reBool("reDeleteSchool",ok,index);
    }
    else if(type == "deleteMajor")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString name = obj.value("name").toString();
        bool ok = MySqlDev::getobject()->deleteMajor(schoolName,name);
        reBool("reDeleteMajor",ok,index);
    }
    else if(type == "deleteClass")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        QString name = obj.value("name").toString();
        bool ok = MySqlDev::getobject()->deleteClass(schoolName,majorName,name);
        reBool("reDeleteClass",ok,index);
    }
    else if(type == "deleteDevice")
    {
        if(!checkAdminKey(obj))
            return;
    }
    else if(type == "updateSchool")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString oldname = obj.value("oldName").toString();
        QString newname = obj.value("newName").toString();
        bool ok = MySqlDev::getobject()->updateSchoolName(oldname,newname);
        reBool("reUpdateSchool",ok,index);
    }
    else if(type == "updateMajor")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString oldname = obj.value("oldName").toString();
        QString newname = obj.value("newName").toString();
        bool ok = MySqlDev::getobject()->updateMajorName(schoolName,oldname,newname);
        reBool("reUpdateMajor",ok,index);
    }
    else if(type == "updateClass")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        QString oldname = obj.value("oldName").toString();
        QString newname = obj.value("newName").toString();
        bool ok = MySqlDev::getobject()->updateClassName(schoolName,majorName,oldname,newname);
        reBool("reUpdateClass",ok,index);
    }
    else if(type == "setClassStateOfRegistration")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        QString className = obj.value("className").toString();
        bool state = obj.value("state").toBool();
        bool ok = MySqlDev::getobject()->setClassStateOfRegistration(schoolName,majorName,className,state);
        reBool("reSetClassStateOfRegistration",ok,index);
    }
    else if(type == "updatePassWord1")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        if(userName != this->userName)
        {
            reBool("reUpdateUserInfo",false,index);
            return;
        }

        QString oldPassWord = obj.value("oldPassWord").toString();
        QString passWord = obj.value("passWord").toString();
        bool ok = MySqlDev::getobject()->updateUserPassword(userName,oldPassWord,passWord);

        reBool("reUpdatePassWord1",ok,index);
        if(ok)
            reLogout(-1);

    }
    else if(type == "getSchoolList")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        reGetSchoolList(index);
    }
    else if(type == "getMajorList")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        reGetMajorList(schoolName, index);
    }
    else if(type == "getClassList")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        reGetClassList(schoolName, majorName, index);
    }
    else if(type == "updateUserInfo1")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        if(userName != this->userName)
        {
            reBool("reUpdateUserInfo",false,index);
            return;
        }
        QString name = obj.value("name").toString();
        QString school = obj.value("school").toString();
        QString major = obj.value("major").toString();
        QString sclass = obj.value("class").toString();
        QString schoolNumber = obj.value("schoolNumber").toString();
        QString post = obj.value("post").toString();
        QString contactInfo = obj.value("contactInfo").toString();
        bool ok = MySqlDev::getobject()->updateUserInfo(userName,name,school,major,sclass,schoolNumber,post,contactInfo);
        if(ok)
            this->isAcessToOperation = MySqlDev::getobject()->checkUserOperationJurisdiction(userName);
        reBool("reUpdateUserInfo",ok,index);
    }
    else if(type == "updateUserInfo2")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        if(userName != this->userName)
        {
            reBool("reUpdateUserInfo",false,index);
            return;
        }
        QString name = obj.value("name").toString();
        QString schoolNumber = obj.value("schoolNumber").toString();
        QString post = obj.value("post").toString();
        QString contactInfo = obj.value("contactInfo").toString();
        bool ok = MySqlDev::getobject()->updateUserInfo(userName,name,schoolNumber,post,contactInfo);
        reBool("reUpdateUserInfo",ok,index);
    }
    else if(type == "updateUserClass")
    {
        if(!checkUserKey(obj))
            return;
    }
    else if(type == "logout")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        reLogout(index);
    }
    else if(type == "getUserList1")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        reGetUserList(schoolName,index);
    }
    else if(type == "getUserList2")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        reGetUserList(schoolName,majorName,index);
    }
    else if(type == "getUserList3")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString schoolName = obj.value("schoolName").toString();
        QString majorName = obj.value("majorName").toString();
        QString className = obj.value("className").toString();
        reGetUserList(schoolName,majorName,className,index);
    }
    else if(type == "getErrorUserList")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        reGetUserList(index);
    }
    else if(type == "getUserInfo")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        reGetUserInfo(userName,index);
    }
    else if(type == "getMyInfo")
    {
        if(!checkUserKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        reGetMyInfo(index);
    }
    else if(type == "updateUserInfo3")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        QString name = obj.value("name").toString();
        QString school = obj.value("school").toString();
        QString major = obj.value("major").toString();
        QString sclass = obj.value("class").toString();
        QString schoolNumber = obj.value("schoolNumber").toString();
        QString post = obj.value("post").toString();
        QString contactInfo = obj.value("contactInfo").toString();
        bool ok = MySqlDev::getobject()->updateUserInfo(userName,name,school,major,sclass,schoolNumber,post,contactInfo);
        reBool("reUpdateUserInfo",ok,index);
    }
    else if(type == "updateUserInfo4")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        QString name = obj.value("name").toString();
        QString schoolNumber = obj.value("schoolNumber").toString();
        QString post = obj.value("post").toString();
        QString contactInfo = obj.value("contactInfo").toString();
        bool ok = MySqlDev::getobject()->updateUserInfo(userName,name,schoolNumber,post,contactInfo);
        reBool("reUpdateUserInfo",ok,index);
    }
    else if(type == "deleteUser")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        QString userName = obj.value("userName").toString();
        bool ok = MySqlDev::getobject()->deleteUser(userName);
        reBool("reDeleteUser",ok,index);
    }
    else if(type == "getAdminList")
    {
        if(!checkAdminKey(obj))
            return;
        int index = obj.value("requestIndex").toInt();
        reGetAdminList(index);
    }
    else if(type == "requestData")
    {
        if(!checkUserKey(obj))
            return;
        if(!isAcessToOperation)
            return;
        int index = obj.value("requestIndex").toInt();
        int id = obj.value("deviceID").toInt();
        QString password = obj.value("passWord").toString();
        emit requestSensingData(socketID,index,id,password);
    }
    else if(type == "controlCmd")
    {
        if(!checkUserKey(obj))
            return;
        if(!isAcessToOperation)
            return;
        Journal::recordData(2,tr("User control,cmd type:%1.")
                            .arg(obj.value("cmdType").toString()));
        emit controlCmd(socketID,obj);
    }
}

void TcpSocket::serverOneUserLoginSucceedSlot(const int i, const QString &cUserName)
{
    if( (socketID != i) && (cUserName == userName) )
    {
        QJsonObject obj;
        obj.insert("type","dropLine");
        sendData(obj);
        userKey = Encryption::generateUserIdentity("Unspecified");
        userName.clear();
    }
}

void TcpSocket::serverOneAdminLoginSucceedSlot(const int i, const QString &cAdminName)
{
    if( (socketID != i) && (cAdminName == adminName) )
    {
        QJsonObject obj;
        obj.insert("type","dropLine");
        sendData(obj);
        userKey = Encryption::generateUserIdentity("Unspecified");
        adminKey = Encryption::generateUserIdentity("Unspecified");
        adminName.clear();
    }
}

void TcpSocket::reRequestSensingDataSlot(int sptr, int index, bool re, const QJsonObject &data)
{
    if(sptr == socketID)
    {
        QJsonObject obj;
        obj.insert("type","reRequestData");
        obj.insert("requestIndex",index);
        obj.insert("re",re);
        if(re)
        {
            obj.insert("data",data);
        }
        else
            Journal::recordData(1,tr("User request sensing data failed,ipaddr:%1,username:%2.")
                                .arg(this->peerAddress().toString())
                                .arg(userName));
        sendData(obj);
    }
}

void TcpSocket::reControlCmdSlot(int sptr, int index, bool re)
{
    if(sptr != socketID)
        return;
    QJsonObject obj;
    obj.insert("type","reControlCmd");
    obj.insert("requestIndex",index);
    obj.insert("re",re);
    if(!re)
        Journal::recordData(1,tr("User control sensing data failed,ipaddr:%1,username:%2.")
                            .arg(this->peerAddress().toString())
                            .arg(userName));
    sendData(obj);
}

void TcpSocket::reLogin(bool re, QString key, int index)
{
    QJsonObject obj;
    obj.insert("type","reLogin");
    obj.insert("re",re);
    obj.insert("requestIndex",index);
    if(re)
    {
        obj.insert("key",key);
        emit oneUserLoginSucceed(socketID,userName);
        Journal::recordData(2,tr("UserLogin %1 successed!").arg(userName));
    }
    else
    {
        Journal::recordData(1,tr("UserLogin failed,ip:%1").arg(this->peerAddress().toString()));
    }
    sendData(obj);
}

void TcpSocket::reRegister(bool re, int index)
{
    QJsonObject obj;
    obj.insert("type","reRegister");
    obj.insert("re",re);
    if(re)
    {
        Journal::recordData(2,tr("Register successed,ip:%1").arg(this->peerAddress().toString()));
    }
    else
    {
        Journal::recordData(1,tr("Register failed,ip:%1").arg(this->peerAddress().toString()));
    }
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reLogout(int index)
{
    userKey = Encryption::generateUserIdentity("Unspecified");
    userName.clear();
    adminKey = Encryption::generateUserIdentity("Unspecified");
    adminName.clear();
    QJsonObject obj;
    obj.insert("type","reLogout");
    obj.insert("requestIndex",index);
    obj.insert("re",true);
    sendData(obj);
}

void TcpSocket::reGetSchoolList(int index)
{
    QJsonObject obj;
    obj.insert("type","reGetSchoolList");
    obj.insert("re",MySqlDev::getobject()->getSchoolList());
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetMajorList(QString schoolName, int index)
{
    QJsonObject obj;
    obj.insert("type","reGetMajorList");
    obj.insert("re",MySqlDev::getobject()->getMajorList(schoolName));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetClassList(QString schoolName, QString majorName, int index)
{
    QJsonObject obj;
    obj.insert("type","reGetClassList");
    obj.insert("re",MySqlDev::getobject()->getClassList(schoolName,majorName));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reBool(QString type, bool ok, int index)
{
    QJsonObject obj;
    obj.insert("type",type);
    obj.insert("re",ok);
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetUserList(QString schoolName, QString majorName, QString className, int index)
{
    QJsonObject obj;
    obj.insert("type","reGetUserList");
    obj.insert("re",MySqlDev::getobject()->getUserList( schoolName,majorName,className));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetUserList(QString schoolName, QString majorName, int index)
{
    QJsonObject obj;
    obj.insert("type","reGetUserList");
    obj.insert("re",MySqlDev::getobject()->getUserList(schoolName,majorName));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetUserList(QString schoolName, int index)
{
    QJsonObject obj;
    obj.insert("type","reGetUserList");
    obj.insert("re",MySqlDev::getobject()->getUserList(schoolName));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetUserList(int index)
{
    QJsonObject obj;
    obj.insert("type","reGetUserList");
    obj.insert("re",MySqlDev::getobject()->getErrorUserList());
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetAdminList(int index)
{
    QJsonObject obj;
    obj.insert("type","reGetAdminList");
    obj.insert("re",MySqlDev::getobject()->getAdminList());
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reAddNewDevice(int id, bool ok, int index)
{
    QJsonObject obj;
    obj.insert("type","reAddNewDevice");
    obj.insert("id",id);
    obj.insert("re",ok);
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetUserInfo(QString userName, int index)
{
    QJsonObject obj;
    obj.insert("type","reGetUserInfo");
    obj.insert("re",MySqlDev::getobject()->getUserInfo(userName));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reGetMyInfo(int index)
{
    QJsonObject obj;
    obj.insert("type","reGetMyInfo");
    obj.insert("re",MySqlDev::getobject()->getUserInfo(userName));
    obj.insert("requestIndex",index);
    sendData(obj);
}

void TcpSocket::reAdminLogin(bool re, QString key, QString adminKey, int index)
{
    QJsonObject obj;
    obj.insert("type","reAdminLogin");
    obj.insert("re",re);
    obj.insert("requestIndex",index);
    if(re)
    {
        obj.insert("key",key);
        obj.insert("adminKey",adminKey);
        emit oneAdminLoginSucceed(socketID,adminName);
        Journal::recordData(2,tr("AdminLogin %1 successed!").arg(userName));
    }
    else
    {
        Journal::recordData(1,tr("AdminLogin failed,ip:%1").arg(this->peerAddress().toString()));
    }
    sendData(obj);
}

void TcpSocket::reAdminRegister(bool re, int index)
{
    QJsonObject obj;
    obj.insert("type","reAdminRegister");
    obj.insert("re",re);
    if(re)
    {
        Journal::recordData(2,tr("Register admin successed,ip:%1").arg(this->peerAddress().toString()));
    }
    else
    {
        Journal::recordData(1,tr("Register admin failed,ip:%1").arg(this->peerAddress().toString()));
    }
    obj.insert("requestIndex",index);
    sendData(obj);
}
