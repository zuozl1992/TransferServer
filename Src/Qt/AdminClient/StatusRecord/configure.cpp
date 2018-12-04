#include <QFile>
#include <QMutex>
#include <QSettings>
#include "configure.h"
#define SETFILE "Configure.ini"
#define NETWORKGROUP "NetWork"
#define IPADDR "IpAddr"
#define DEFUSERIP "127.0.0.1"
#define PORT "Port"
#define DEFPASSPORT 10000

Configure::Configure(QObject *parent) : QObject(parent)
{
    initSettingFile();
}

Configure *Configure::getObject()
{
    static Configure cf;
    return &cf;
}

QString Configure::getNetWorkIpAddr()
{
    static QMutex cfmutex;
    QMutexLocker locker(&cfmutex);
    QSettings set(SETFILE,QSettings::IniFormat);
    set.beginGroup(NETWORKGROUP);
    QString re = set.value(IPADDR).toString();
    set.endGroup();
    return re;
}

qint16 Configure::getNetWorkPort()
{
    static QMutex cfmutex;
    QMutexLocker locker(&cfmutex);
    QSettings set(SETFILE,QSettings::IniFormat);
    set.beginGroup(NETWORKGROUP);
    qint16 re = set.value(PORT).toInt();
    set.endGroup();
    return re;
}

void Configure::initSettingFile()
{
    if(QFile(SETFILE).exists())
        return;
    QSettings set(SETFILE,QSettings::IniFormat);
    set.beginGroup(NETWORKGROUP);
    set.setValue(IPADDR,DEFUSERIP);
    set.setValue(PORT,DEFPASSPORT);
    set.endGroup();
}
