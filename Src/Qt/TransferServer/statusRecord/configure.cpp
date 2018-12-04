#include <QFile>
#include <QMutex>
#include <QSettings>
#include <QJsonObject>
#include "configure.h"
#define SETFILE         "Configure.ini"
#define SQLGROUP        "Mysql"
#define SQLHOST         "SqlHost"
#define SQLPORT         "SqlPort"
#define SQLUSERNAME     "SqlUserName"
#define SQLPASSWORD     "SqlPassWord"
#define SQLDATABASE     "SqlDataBase"

#define NETWORKGROUP    "NetWork"
#define NTUMPORT        "NTUMPort"
#define NTDEVPORT       "NTDevPort"

#define STATUSGROUP     "Status"
#define STINFOSWITCH    "STInfoSwitch"
#define STJOURNALLEVEL  "STJournalLevel"
#define STJOURNALCATLEN "STJournalCatLen"

Configure::Configure(QObject *parent) : QObject(parent)
{
    initSettingFile();
}

QJsonObject Configure::getSQLConf()
{
    return Configure::getObject()->prGetSQLConf();
}

QJsonObject Configure::getNetConf()
{
    return Configure::getObject()->prGetNetConf();
}

QJsonObject Configure::getStatusConf()
{
    return Configure::getObject()->prGetStatusConf();
}

Configure *Configure::getObject()
{
    static Configure cf;
    return &cf;
}

void Configure::initSettingFile()
{
    if(QFile(SETFILE).exists())
        return;
    QSettings *set = new QSettings(SETFILE,QSettings::IniFormat);
    set->beginGroup(SQLGROUP);
    set->setValue(SQLHOST,"localhost");
    set->setValue(SQLPORT,3306);
    set->setValue(SQLUSERNAME,"root");
    set->setValue(SQLPASSWORD,"");
    set->setValue(SQLDATABASE,"mysql");
    set->endGroup();
    set->beginGroup(NETWORKGROUP);
    set->setValue(NTUMPORT,10001);
    set->setValue(NTDEVPORT,10002);
    set->endGroup();
    set->beginGroup(STATUSGROUP);
    set->setValue(STINFOSWITCH,true);
    set->setValue(STJOURNALLEVEL,1);
    set->setValue(STJOURNALCATLEN,1000);
    set->endGroup();
}

QJsonObject Configure::prGetSQLConf()
{
    static QMutex prMutex;
    QMutexLocker locker(&prMutex);
    QSettings *set = new QSettings(SETFILE,QSettings::IniFormat);
    set->beginGroup(SQLGROUP);
    QJsonObject re;
    re.insert(SQLHOST,set->value(SQLHOST).toString());
    re.insert(SQLPORT,set->value(SQLPORT).toInt());
    re.insert(SQLUSERNAME,set->value(SQLUSERNAME).toString());
    re.insert(SQLPASSWORD,set->value(SQLPASSWORD).toString());
    re.insert(SQLDATABASE,set->value(SQLDATABASE).toString());
    set->endGroup();
    return re;
}

QJsonObject Configure::prGetNetConf()
{
    static QMutex prMutex;
    QMutexLocker locker(&prMutex);
    QSettings *set = new QSettings(SETFILE,QSettings::IniFormat);
    set->beginGroup(NETWORKGROUP);
    QJsonObject re;
    re.insert(NTUMPORT,set->value(NTUMPORT).toInt());
    re.insert(NTDEVPORT,set->value(NTDEVPORT).toInt());
    set->endGroup();
    return re;
}

QJsonObject Configure::prGetStatusConf()
{
    static QMutex prMutex;
    QMutexLocker locker(&prMutex);
    QSettings *set = new QSettings(SETFILE,QSettings::IniFormat);
    set->beginGroup(STATUSGROUP);
    QJsonObject re;
    re.insert(STINFOSWITCH,set->value(STINFOSWITCH).toBool());
    re.insert(STJOURNALLEVEL,set->value(STJOURNALLEVEL).toInt());
    re.insert(STJOURNALCATLEN,set->value(STJOURNALCATLEN).toInt());
    set->endGroup();
    return re;
}
