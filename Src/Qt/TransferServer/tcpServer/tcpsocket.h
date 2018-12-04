#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QJsonObject>
#include <QTime>
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();


signals:
    //接受到信号
    void readData(const int,const QString &,const quint16,const QJsonObject &);
    //NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);
    //新用户成功登录
    void oneUserLoginSucceed(const int,const QString &);
    //新管理员成功登录
    void oneAdminLoginSucceed(const int,const QString &);
    //一条新信息接收结束
    void oneFullDataRead(const QByteArray &data);
    //请求数据
    void requestSensingData(int sptr,int index,int id,const QString &pass);
    //控制
    void controlCmd(int sptr,const QJsonObject &cmd);

protected slots:
    //发送信号的槽
    void sentData(const QJsonObject & ,const int);
    //断开连接
    void disConTcp(int i);
    //接收数据
    void readDataSlot();
    //断开连接
    void disconnectedSlot();
    //处理下一个接收数据
    void startNext();
    //完整数据接收完成
    void oneFullDataReadSlot(const QByteArray &data);
    //新用户登录
    void serverOneUserLoginSucceedSlot(const int,const QString &);
    //新管理员登录
    void serverOneAdminLoginSucceedSlot(const int,const QString &);
    //传感数据请求返回
    void reRequestSensingDataSlot(int sptr,int index,bool re,const QJsonObject &data);
    //控制结果返回
    void reControlCmdSlot(int sptr,int index,bool re);

protected:
    //用来处理数据的函数
    void handleData(const QByteArray &data);
    //发送数据
    void sendData(const QJsonObject &);
    //校验管理员密钥
    bool checkAdminKey(const QJsonObject &obj);
    //校验用户密钥
    bool checkUserKey(const QJsonObject &obj);
    //错误信息返回
    void reError(const QString &data,int index = -1);
    //登录返回
    void reLogin(bool re,QString key,int index);
    //注册返回处理
    void reRegister(bool re,int index);
    //注销返回处理
    void reLogout(int index);
    //返回学校列表
    void reGetSchoolList(int index);
    //返回专业列表
    void reGetMajorList(QString schoolName, int index);
    //返回班级类别
    void reGetClassList(QString schoolName, QString majorName, int index);
    //操作结果
    void reBool(QString type, bool ok, int index);
    //用户列表
    void reGetUserList(QString schoolName,QString majorName,QString className,int index);
    void reGetUserList(QString schoolName,QString majorName,int index);
    void reGetUserList(QString schoolName,int index);
    //未激活用户
    void reGetUserList(int index);
    //管理员列表
    void reGetAdminList(int index);
    //添加设备
    void reAddNewDevice(int id,bool ok,int index);
    //获取用户信息
    void reGetUserInfo(QString userName,int index);

    void reGetMyInfo(int index);
    //管理员登录
    void reAdminLogin(bool re,QString key,QString adminKey,int index);
    //管理员注册
    void reAdminRegister(bool re,int index);

    QFutureWatcher<void> rawDataWatcher;
    QQueue<QByteArray> rawDataQueue;
    qintptr socketID;
    QMetaObject::Connection dis;
    QString userKey;
    QString userName;
    QString adminName;
    QString adminKey;
    QByteArray readBuffer;
    bool isAcessToOperation = false;    //账户是否激活
    quint8 lastChar = ' ';
    qint32 braNum = 0;
};

#endif // TCPSOCKET_H
