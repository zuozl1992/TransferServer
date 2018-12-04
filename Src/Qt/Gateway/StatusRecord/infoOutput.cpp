#include <QDebug>
#include <QMutex>
#include <QJsonDocument>
#include "infoOutput.h"

#define cout *out
#define cerr *err
InfoOutput::InfoOutput(QObject *parent) : QObject(parent)
{
    out = new QTextStream(stdout);
    err = new QTextStream(stderr);
    prPutOutInfo("InfoOutput init");
}

InfoOutput *InfoOutput::getobject()
{
    static InfoOutput iop;
    return &iop;
}

void InfoOutput::putOutInfo(const QString &cData)
{
    InfoOutput::getobject()->prPutOutInfo(cData);
}

void InfoOutput::putOutInfo(const QString &cType, const QJsonObject &cObj)
{
    InfoOutput::getobject()->prPutOutInfo(cType,cObj);
}

void InfoOutput::putOutNum(qint64 num)
{
    InfoOutput::getobject()->prPutOutNum(num);
}

void InfoOutput::putOutErrorInfo(const QString &cErrorData)
{
    InfoOutput::getobject()->prPutOutErrorData(cErrorData);
}

void InfoOutput::putOutErrorNum(qint64 errorNum)
{
    InfoOutput::getobject()->prPutOutErrorNum(errorNum);
}

void InfoOutput::prPutOutInfo(const QString &cData)
{
    static QMutex iopmutex;
    QMutexLocker locker(&iopmutex);
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cout << str << " " << cData << endl;
}

void InfoOutput::prPutOutInfo(const QString &cType, const QJsonObject &cObj)
{
    static QMutex iopmutex;
    QMutexLocker locker(&iopmutex);
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cout << str << cType << " " << endl;
    prPutOutJson(cObj);
}

void InfoOutput::prPutOutNum(qint64 num)
{
    static QMutex iopmutex;
    QMutexLocker locker(&iopmutex);
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cout << str << " " << tr("%1").arg(num) << endl;
}

void InfoOutput::prPutOutErrorData(const QString &cErrorData)
{
    static QMutex iopmutex;
    QMutexLocker locker(&iopmutex);
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cerr << str << " Error: " << cErrorData << endl;
}

void InfoOutput::prPutOutErrorNum(qint64 errorNum)
{
    static QMutex iopmutex;
    QMutexLocker locker(&iopmutex);
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    cerr << str << " Error: " << tr("%1").arg(errorNum) << endl;
}

void InfoOutput::prPutOutJson(const QJsonObject &cObj)
{
    cout << QJsonDocument(cObj).toJson() << endl;
}
