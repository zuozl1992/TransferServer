#include <QMutex>
#include <QJsonDocument>
#include <QTextStream>
#include <QDateTime>
#include "infoOutput.h"

#define cout *out
#define cerr *err

InfoOutput::InfoOutput(QObject *parent) : QObject(parent)
{
    out = new QTextStream(stdout);
    err = new QTextStream(stderr);
}

void InfoOutput::putOutInfo(const QString &cData)
{
    InfoOutput::getobject()->proPutOutInfo(cData);
}

void InfoOutput::putOutInfo(qint64 num)
{
    InfoOutput::getobject()->proPutOutInfo(tr("%1").arg(num));
}

void InfoOutput::putOutInfo(const QString &cData, bool re)
{
    InfoOutput::getobject()->proPutOutInfo(cData,re);
}

void InfoOutput::putOutInfo(const QString &cType, const QJsonObject &cObj)
{
    InfoOutput::getobject()->proPutOutInfo(cType,cObj);
}

void InfoOutput::putOutErrorInfo(const QString &cErrorData)
{
    InfoOutput::getobject()->proPutOutErrorInfo("Error",cErrorData);
}

void InfoOutput::putOutErrorInfo(qint64 num)
{
    InfoOutput::getobject()->proPutOutErrorInfo("Error",tr("%1").arg(num));
}

void InfoOutput::putOutErrorInfo(const QString &cData, bool re)
{
    InfoOutput::getobject()->proPutOutErrorInfo("Error",cData,re);
}

void InfoOutput::putOutWarningInfo(const QString &cErrorData)
{
    InfoOutput::getobject()->proPutOutErrorInfo("Warning",cErrorData);
}

void InfoOutput::putOutWarningInfo(qint64 num)
{
    InfoOutput::getobject()->proPutOutErrorInfo("Warning",tr("%1").arg(num));
}

void InfoOutput::putOutWarningInfo(const QString &cData, bool re)
{
    InfoOutput::getobject()->proPutOutErrorInfo("Warning",cData,re);
}

void InfoOutput::setDisplaySwitch(bool sw)
{
    InfoOutput::getobject()->proSetDisplaySwitch(sw);
}

InfoOutput *InfoOutput::getobject()
{
    static InfoOutput iop;
    return &iop;
}

void InfoOutput::proPutOutInfo(const QString &cData)
{
    static QMutex iopMutex;
    QMutexLocker locker(&iopMutex);
    if(!displaySwitch) return;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cout << str << " " << cData << endl;
}

void InfoOutput::proPutOutInfo(const QString &cType, const QJsonObject &cObj)
{
    static QMutex iopMutex;
    QMutexLocker locker(&iopMutex);
    if(!displaySwitch) return;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cout << str << cType << " " << endl;
    proPutOutJson(cObj);
}

void InfoOutput::proPutOutInfo(const QString &cData, bool re)
{
    static QMutex iopMutex;
    QMutexLocker locker(&iopMutex);
    if(!displaySwitch) return;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cout << str << " " << cData << " : " << re << endl;
}

void InfoOutput::proPutOutErrorInfo(QString type,const QString &cErrorData)
{
    static QMutex iopMutex;
    QMutexLocker locker(&iopMutex);
    if(!displaySwitch) return;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cerr << str << " " << type << ": " << cErrorData << endl;
}

void InfoOutput::proPutOutErrorInfo(QString type,const QString &cData, bool re)
{
    static QMutex iopMutex;
    QMutexLocker locker(&iopMutex);
    if(!displaySwitch) return;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cerr << str << " " << type << ": " << cData << " : " << re << endl;
}

void InfoOutput::proPutOutJson(const QJsonObject &value)
{
    cout << QJsonDocument(value).toJson();
}

void InfoOutput::proSetDisplaySwitch(bool sw)
{
    static QMutex iopMutex;
    QMutexLocker locker(&iopMutex);
    displaySwitch = sw;
}
