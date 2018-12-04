#ifndef TCPSOCKETDEV_H
#define TCPSOCKETDEV_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QJsonObject>
#include <QTime>

class TcpSocketDev : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocketDev(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocketDev();

signals:
    //接受到信号
    void readData(const int,const QString &,const quint16,const QJsonObject &);
    //NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);
    //一个用户成功登录
    void oneDeviceLoginSucceed(int,int);
    //新数据接收完成
    void oneFullDataRead(const QByteArray &data);
    //请求返回
    void reRequestSensingData(int sptr,int index,bool re,const QJsonObject &data);
    //控制返回
    void reControlCmd(int sptr,int index,bool re);

protected slots:
    //发送信号的槽
    void sentData(const QJsonObject & ,const int);
    //断开连接
    void disConTcp(int i);
    //接收数据
    void readDataSlot();
    //断开连接
    void disconnectedSlot();
    //处理下一个
    void startNext();
    //一帧完整数据接收
    void oneFullDataReadSlot(const QByteArray &data);
    //设备登录
    void serverOneDeviceLoginSucceedSlot(int i,int deviceID);
    //请求到达
    void requestSensingDataSlot(int sptr,int index,int id,const QString &pass);
    //控制到达
    void controlCmdSlot(int sptr,const QJsonObject &cmd);

protected:
    //用来处理数据的函数
    void handleData(const QByteArray &data);
    //发送数据
    void sendData(const QJsonObject &);
    //错误信息返回
    void reError();
    //新设备登录
    void reDeviceLogin(int index,bool re,const QString &key);
    //校验设备数据
    bool checkDeviceKey(const QJsonObject &obj);

    QFutureWatcher<void> rawDataWatcher;
    QQueue<QByteArray> rawDataQueue;
    qintptr socketID;
    QMetaObject::Connection dis;
    QString deviceKey;
    QString devicePass;
    int deviceID;
    QByteArray readBuffer;
    quint8 lastChar = ' ';
    qint32 braNum = 0;
    QJsonObject sensingData;
};

#endif // TCPSOCKETDEV_H
