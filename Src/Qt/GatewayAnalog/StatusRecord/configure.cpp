#include <QFile>
#include <QMutex>
#include <QSettings>
#include "configure.h"
#include "infoOutput.h"
#define SETFILE "Configure.ini"
#define NETWORKGROUP "NetWork"
#define IPADDR "IpAddr"
#define DEFUSERIP "127.0.0.1"
#define PORT "Port"
#define DEFPASSPORT 10000

#define CLASSGROUP "Class"
#define DEVICEID "DeviceID"
#define DEFDEVICEID 1001
#define DEVICEPASS "DevicePass"
#define DEFDEVICEPASS ""

Configure::Configure(QObject *parent) : QObject(parent)
{
    initSettingFile();
    InfoOutput::putOutInfo("Configure init");
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

quint16 Configure::getNetWorkPort()
{
    static QMutex cfmutex;
    QMutexLocker locker(&cfmutex);
    QSettings set(SETFILE,QSettings::IniFormat);
    set.beginGroup(NETWORKGROUP);
    quint16 re = set.value(PORT).toInt();
    set.endGroup();
    return re;
}

qint32 Configure::getDeviceID()
{
    static QMutex cfmutex;
    QMutexLocker locker(&cfmutex);
    QSettings set(SETFILE,QSettings::IniFormat);
    set.beginGroup(CLASSGROUP);
    qint32 re = set.value(DEVICEID).toInt();
    set.endGroup();
    return re;
}

QString Configure::getDevicePass()
{
    static QMutex cfmutex;
    QMutexLocker locker(&cfmutex);
    QSettings set(SETFILE,QSettings::IniFormat);
    set.beginGroup(CLASSGROUP);
    QString re = set.value(DEVICEPASS).toString();
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
    set.beginGroup(CLASSGROUP);
    set.setValue(DEVICEID,DEFDEVICEID);
    set.setValue(DEVICEPASS,DEFDEVICEPASS);
    set.endGroup();
}
