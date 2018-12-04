#include <QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QMutex>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "mysqldev.h"
#include "infoOutput.h"

MySqlDev::MySqlDev(QObject *parent) : QObject(parent)
{
    pMysql = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    pQuery = new QSqlQuery;
    pUpdateTimer = new QTimer;
    connect(pUpdateTimer,SIGNAL(timeout()),
            this,SLOT(updateTimerTimeoutSlot()));
    pUpdateTimer->start(3600000);
}

MySqlDev::~MySqlDev()
{
}

MySqlDev *MySqlDev::getobject()
{
    static MySqlDev msd;
    return &msd;
}

bool MySqlDev::connectToDatabase(const QString &cHostName, const int &cPort, const QString &cUserName, const QString &cPasswd, const QString &cDatabaseName)
{
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pMysql->isOpen())
    {
        pMysql->close();
    }
    pMysql->setPort(cPort);
    pMysql->setHostName(cHostName);
    pMysql->setUserName(cUserName);
    pMysql->setPassword(cPasswd);
    pMysql->setDatabaseName(cDatabaseName);
    if(pMysql->open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MySqlDev::setClassStateOfRegistration(const QString &cSchoolName, const QString &cMajorName, const QString &cClassName, bool state)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_classInfo SET Type=%1 WHERE SchoolName='%2' AND MajorName='%3' AND Name='%4';")
                    .arg(state?1:0).arg(cSchoolName).arg(cMajorName).arg(cClassName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::setClassStateOfRegistration(int id, bool state)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_classInfo SET Type=%1 WHERE ID=%2;")
                    .arg(state?1:0).arg(id)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

qint64 MySqlDev::getClassID(const QString &cSchoolName, const QString &cMajorName, const QString &cClassName)
{
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT ID FROM t_classInfo WHERE SchoolName='%1' AND MajorName='%2' AND Name='%3';")
                    .arg(cSchoolName).arg(cMajorName).arg(cClassName)))
    {
        while(pQuery->next())
        {
            return pQuery->value(0).toInt();
        }
    }
    return 0;
}

bool MySqlDev::getClassStateOfRegistration(const QString &cSchoolName, const QString &cMajorName, const QString &cClassName)
{
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT Type FROM t_classInfo WHERE SchoolName='%1' AND MajorName='%2' AND Name='%3';")
                    .arg(cSchoolName).arg(cMajorName).arg(cClassName)))
    {
        while(pQuery->next())
        {
            return pQuery->value(0).toInt() == 0 ? false : true;
        }
    }
    return false;
}

bool MySqlDev::getClassStateOfRegistration(int id)
{
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT Type FROM t_classInfo WHERE ID=%1;")
                    .arg(id)))
    {
        while(pQuery->next())
        {
            return pQuery->value(0).toInt() == 0 ? false : true;
        }
    }
    return false;
}

bool MySqlDev::getSqlStat()
{
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT CheckKey FROM t_dynamicCheck WHERE CheckValue='stateCheck';")))
    {
        while(pQuery->next())
        {
            if(pQuery->value(0).toString() == "ok")
            {
                return true;
            }
        }
    }
    return false;
}

bool MySqlDev::addNewUser(const QString &cUserName, const QString &cPasswd, const QString &cRegisterIP)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("INSERT INTO t_userPassword (UserName,PassWord) VALUES('%1', '%2');")
                    .arg(cUserName).arg(cPasswd)))
    {
        if(pQuery->exec(tr("SELECT ID FROM t_userPassword WHERE UserName='%1';").arg(cUserName)))
        {
            while(pQuery->next())
            {
                int id = pQuery->value(0).toInt();
                if(pQuery->exec(tr("INSERT INTO t_userInfo (ID,UserName,RegisterIPAddr) VALUES(%1, '%2', '%3')")
                                .arg(id).arg(cUserName).arg(cRegisterIP)))
                    return true;
                else
                    pQuery->exec(tr("DELETE FROM t_userPassword WHERE UserName='%1';")
                                 .arg(cUserName));
            }
        }
    }
    return false;
}

bool MySqlDev::addNewAdmin(const QString &cUserName, const QString &cPasswd)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("INSERT INTO t_adminPassword (UserName,PassWord) VALUES('%1', '%2');")
                    .arg(cUserName).arg(cPasswd)))
    {
        return true;
    }
    return false;
}

qint32 MySqlDev::addNewDevice(const QString &cDevicePasswd)
{
    if(!getSqlStat())
        return -1;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("INSERT INTO t_devicePassword (PassWord) VALUES('%1');")
                    .arg(cDevicePasswd)))
    {
        if(pQuery->exec("SELECT LAST_INSERT_ID();"))
        {
            while(pQuery->next())
            {
                return pQuery->value(0).toInt();
            }
        }
    }
    return -1;
}

bool MySqlDev::addNewSchool(const QString &cName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("INSERT INTO t_schoolInfo (Name) VALUES('%1');")
                    .arg(cName)))
        return true;
    return false;
}

bool MySqlDev::addNewMajor(const QString &cSchoolName, const QString &cName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("INSERT INTO t_majorInfo (SchoolName,Name) VALUES('%1', '%2');")
                    .arg(cSchoolName)
                    .arg(cName)))
        return true;
    return false;
}

bool MySqlDev::addNewClass(const QString &cSchoolName, const QString &cMajorName, const QString &cName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("INSERT INTO t_classInfo (SchoolName,MajorName,Name) VALUES('%1', '%2', '%3');")
                    .arg(cSchoolName)
                    .arg(cMajorName)
                    .arg(cName)))
        return true;
    return false;
}

QJsonArray MySqlDev::getSchoolList()
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT Name FROM t_schoolInfo;")))
    {
        while(pQuery->next())
        {
            list.append(pQuery->value(0).toString());
        }
    }
    return list;
}

QJsonArray MySqlDev::getMajorList(const QString &cSchoolName)
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT Name FROM t_majorInfo WHERE SchoolName='%1';")
                    .arg(cSchoolName)))
    {
        while(pQuery->next())
        {
            list.append(pQuery->value(0).toString());
        }
    }
    return list;
}

QJsonArray MySqlDev::getClassList(const QString &cSchoolName,const QString &cMajorName)
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_classInfo WHERE SchoolName='%1' AND MajorName='%2';")
                    .arg(cSchoolName)
                    .arg(cMajorName)))
    {
        while(pQuery->next())
        {
            QJsonObject obj;
            obj.insert("name",pQuery->value(3).toString());
            obj.insert("type",pQuery->value(4).toInt()==1?true:false);
            list.append(obj);
        }
    }
    return list;
}

QJsonArray MySqlDev::getUserList(const QString &cSchoolName)
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_userInfo WHERE School='%1';")
                    .arg(cSchoolName)))
    {
        while(pQuery->next())
        {
            QJsonObject info;
            info.insert("userName",pQuery->value(1).toString());
            info.insert("name",pQuery->value(2).toString());
            list.append(info);
        }
    }
    return list;
}

QJsonArray MySqlDev::getUserList(const QString &cSchoolName, const QString &cMajorName)
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_userInfo WHERE School='%1' AND Major='%2';")
                    .arg(cSchoolName)
                    .arg(cMajorName)))
    {
        while(pQuery->next())
        {
            QJsonObject info;
            info.insert("userName",pQuery->value(1).toString());
            info.insert("name",pQuery->value(2).toString());
            list.append(info);
        }
    }
    return list;
}

QJsonArray MySqlDev::getUserList(const QString &cSchoolName, const QString &cMajorName, const QString &cClassName)
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_userInfo WHERE School='%1' AND Major='%2' AND Class='%3';")
                    .arg(cSchoolName)
                    .arg(cMajorName)
                    .arg(cClassName)))
    {
        while(pQuery->next())
        {
            QJsonObject info;
            info.insert("userName",pQuery->value(1).toString());
            info.insert("name",pQuery->value(2).toString());
            list.append(info);
        }
    }
    return list;
}

QJsonObject MySqlDev::getUserInfo(const QString &cUserName)
{
    QJsonObject info;
    if(!getSqlStat())
        return info;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_userInfo WHERE UserName='%1';")
                    .arg(cUserName)))
    {
        while (pQuery->next()) {
            info.insert("name",pQuery->value(2).toString());
            info.insert("school",pQuery->value(3).toString());
            info.insert("major",pQuery->value(4).toString());
            info.insert("class",pQuery->value(5).toString());
            info.insert("schoolNumber",pQuery->value(6).toString());
            info.insert("post",pQuery->value(7).toString());
            info.insert("contactInfo",pQuery->value(8).toString());
            return info;
        }
    }
    return info;
}

QJsonArray MySqlDev::getErrorUserList()
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_userInfo WHERE School IS NULL;")
                    ))
    {
        while(pQuery->next())
        {
            QJsonObject info;
            info.insert("userName",pQuery->value(1).toString());
            info.insert("name",pQuery->value(2).toString());
            list.append(info);
        }
    }
    return list;
}

bool MySqlDev::userLogin(const QString &cUserName, const QString &cPassWord)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT PassWord FROM t_userPassword WHERE UserName='%1';")
                    .arg(cUserName)))
    {
        while(pQuery->next())
        {
            return pQuery->value(0).toString() == cPassWord ? true : false;
        }
    }
    return false;
}

bool MySqlDev::adminLogin(const QString &cUserName, const QString &cPassWord)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT PassWord FROM t_adminPassword WHERE UserName='%1';")
                    .arg(cUserName)))
    {
        while(pQuery->next())
        {
            return pQuery->value(0).toString() == cPassWord ? true : false;
        }
    }
    return false;
}

bool MySqlDev::checkUserOperationJurisdiction(const QString &cUserName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT Class FROM t_userInfo WHERE UserName='%1';").arg(cUserName)
                    ))
    {
        while(pQuery->next())
        {
            if(!pQuery->value(0).isNull())
                return true;
        }
    }
    return false;
}

bool MySqlDev::deviceCheck(int id, const QString &cPassWord)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT PassWord FROM t_devicePassword WHERE ID=%1;")
                    .arg(id)))
    {
        while(pQuery->next())
        {
            return pQuery->value(0).toString() == cPassWord ? true : false;
        }
    }
    return false;
}

QJsonArray MySqlDev::getAdminList()
{
    QJsonArray list;
    if(!getSqlStat())
        return list;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("SELECT * FROM t_adminPassword;")
                    ))
    {
        while(pQuery->next())
        {
            list.append(pQuery->value(0).toString());
        }
    }
    return list;
}

bool MySqlDev::updateUserPassword(const QString &cUserName, const QString &cOldPasswd, const QString &cPasswd)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    bool check = false;
    if(pQuery->exec(tr("SELECT PassWord FROM t_userPassword WHERE UserName='%1'")
                    .arg(cUserName)))
    {
        while (pQuery->next()) {
            if(pQuery->value(0).toString() == cOldPasswd)
                check = true;
        }
    }
    if(!check)
        return false;
    if(pQuery->exec(tr("UPDATE t_userPassword SET PassWord='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cPasswd)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserPassword(const QString &cUserName, const QString &cPasswd)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userPassword SET PassWord='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cPasswd)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateAdminPassword(const QString &cUserName, const QString &cPasswd)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_adminPassword SET PassWord='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cPasswd)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserInfo(const QString &cUserName, const QString &cName, const QString &cSchool, const QString &cMajor, const QString &cClass, const QString &cSchoolNumber, const QString &cPost, const QString &cContactInfo)
{
    if(!getSqlStat())
        return false;
    if(!getClassStateOfRegistration(cSchool,cMajor,cClass))
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET Name='%2',School='%3',Major='%4',Class='%5',SchoolNumber='%6',Post='%7',ContactInfo='%8' WHERE UserName='%1';")
                    .arg(cUserName).arg(cName)
                    .arg(cSchool).arg(cMajor).arg(cClass).arg(cSchoolNumber)
                    .arg(cPost).arg(cContactInfo)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserInfo(const QString &cUserName, const QString &cName, const QString &cSchoolNumber, const QString &cPost, const QString &cContactInfo)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET Name='%2',SchoolNumber='%3',Post='%4',ContactInfo='%5' WHERE UserName='%1';")
                    .arg(cUserName).arg(cName).arg(cSchoolNumber)
                    .arg(cPost).arg(cContactInfo)))
    {
        if(pQuery->numRowsAffected() > 0)
            return true;
    }
    qDebug() << cUserName << " " << cName;
    return false;
}

bool MySqlDev::updateUserName(const QString &cUserName, const QString &cName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET Name='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserClass(const QString &cUserName, const QString &cSchool, const QString &cMajor, const QString &cClass)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET School='%2',Major='%3',Class='%4' WHERE UserName='%1';")
                    .arg(cUserName).arg(cSchool).arg(cMajor).arg(cClass)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserSchoolNumber(const QString &cUserName, const QString &cSchoolNumber)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET SchoolNumber='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cSchoolNumber)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserPost(const QString &cUserName, const QString &cPost)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET Post='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cPost)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateUserContactInfo(const QString &cUserName, const QString &cContactInfo)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_userInfo SET ContactInfo='%2' WHERE UserName='%1';")
                    .arg(cUserName).arg(cContactInfo)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateSchoolName(const QString &cOldName, const QString &cNewName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_schoolInfo SET Name='%2' WHERE Name='%1';")
                    .arg(cOldName).arg(cNewName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateMajorName(const QString &cSchoolName, const QString &cOldName, const QString &cNewName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_majorInfo SET Name='%3' WHERE SchoolName='%1' AND Name='%2';")
                    .arg(cSchoolName).arg(cOldName).arg(cNewName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::updateClassName(const QString &cSchoolName, const QString &cMajorName, const QString &cOldName, const QString &cNewName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("UPDATE t_classInfo SET Name='%4' WHERE SchoolName='%1' AND MajorName='%2' AND Name='%3';")
                    .arg(cSchoolName).arg(cMajorName).arg(cOldName).arg(cNewName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::deleteUser(const QString &cUserName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("DELETE FROM t_userPassword WHERE UserName='%1';")
                    .arg(cUserName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::deleteSchool(const QString &cSchoolName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("DELETE FROM t_schoolInfo WHERE Name='%1';")
                    .arg(cSchoolName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::deleteMajor(const QString &cSchoolName, const QString &cMajorName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("DELETE FROM t_majorInfo WHERE SchoolName='%1' AND Name='%2';")
                    .arg(cSchoolName)
                    .arg(cMajorName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

bool MySqlDev::deleteClass(const QString &cSchoolName, const QString &cMajorName, const QString &cClassName)
{
    if(!getSqlStat())
        return false;
    static QMutex msdMutex;
    QMutexLocker locker(&msdMutex);
    if(pQuery->exec(tr("DELETE FROM t_classInfo WHERE SchoolName='%1' AND MajorName='%2' AND Name='%3';")
                    .arg(cSchoolName)
                    .arg(cMajorName)
                    .arg(cClassName)))
        if(pQuery->numRowsAffected() > 0)
            return true;
    return false;
}

void MySqlDev::updateTimerTimeoutSlot()
{
    getSqlStat();
}
