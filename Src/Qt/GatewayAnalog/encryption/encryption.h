/*
 * ClassName：Encryption
 * Description：用于提供相关的加密算法
 *
 * Author：Albert
 * Version：V1.0
 * Date：2018.09.18
 * Version Description：
 *          【2018.09.18】 V1.0
 *              1.调整接口结构
 *              2.修改接口名称
 *          【2017.07.27】 V0.2
 *              1.提供获取单例类对象的静态函数。
 *              2.增加生成用户密钥的类成员函数，该函数会根据传入的用户名、当前的系统时间、当前的顺序校验符生成一次性的，5个字符的用户身份密钥。
 *                该密钥每次用户登陆一次性授予，除登陆外的所有操作，用户需要根据的登陆时系统授予的一次性密钥对发送数据进行校验，并将校验结构随通信数据一同上传服务器，服务器才会响应对应操作。
 *              3.添加了获取系统时间的QTime类对象。
 *              4.添加了用于顺序校验的int型变量，每次使用，该数应当加一，当该数大于100时，应当对其赋值为0。
 *          【2017.07.16】 V0.1
 *              1.创建该类。
 *              2.将类构造函数改为私有类型，暂时不允许新建该类对象。
 *              3.添加静态函数用于生成指定字符串的MD5加密字符串。
 */
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QObject>

class Encryption : public QObject
{
    Q_OBJECT
public:
    static Encryption * getobject();
    //[static]获取指定字符串的mdk加密数据
    //@param cStr 要加密的字符串
    //@return QString 加密结果
    static QString md5(const QString &cStr);
    static QString sha1(const QString &cStr);
    //[static]获取用户密钥 10位
    //@param cName 用户名
    //@return QString 密钥
    static QString generateUserIdentity(const QString &cName);

private:
    explicit Encryption(QObject *parent = 0);

    QString userIdentity(QString);
    int iCheckKey = 0;

};

#endif // ENCRYPTION_H
