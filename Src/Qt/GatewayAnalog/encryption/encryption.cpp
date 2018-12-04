#include <QCryptographicHash>
#include <QDateTime>
#include <QMutex>
#include "encryption.h"

QString Encryption::md5(const QString &cStr)
{
    QByteArray bytePwd;
    bytePwd = cStr.toLocal8Bit();
    QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
    QString strPwdMd5 = bytePwdMd5.toHex();
    return strPwdMd5;
}

QString Encryption::sha1(const QString &cStr)
{
    QByteArray bytePwd;
    bytePwd = cStr.toLocal8Bit();
    QByteArray bytePwdSha1 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Sha1);
    QString strPwdMd5 = bytePwdSha1.toHex();
    return strPwdMd5;
}

QString Encryption::generateUserIdentity(const QString &strUserName)
{
    return Encryption::getobject()->userIdentity(strUserName);
}

Encryption *Encryption::getobject()
{
    static Encryption enc;
    return &enc;
}

Encryption::Encryption(QObject *parent) : QObject(parent)
{
}

QString Encryption::userIdentity(QString strPwd5)
{
    static QMutex encMutex;
    QMutexLocker locker(&encMutex);
    //为待处理字符串添加毫秒级当前的系统时间
    QString str = QDateTime::currentDateTime().toString("zzzyyyyzzzhhmmddMMss");
    //为待处理字符串添加顺序校验符
    str += tr("%1").arg(iCheckKey++);
    if(iCheckKey >100)
        iCheckKey=0;
    strPwd5.append(str);
    //对该字符串进行MD5加密
    QByteArray bytePwd;
    bytePwd = strPwd5.toLocal8Bit();
    QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
    QString strPwdMd5 = bytePwdMd5.toHex();
    QString strCheck = "UserIdentityCheck";
    //对将要输出的字符串进行长度判断，保证能够输出固定长度的结果。
    if(strPwdMd5.length() > 10)
        return strPwdMd5.mid(0,10);
    else
        return (strPwdMd5 + strCheck.mid(0,10-strPwdMd5.size()));
}
