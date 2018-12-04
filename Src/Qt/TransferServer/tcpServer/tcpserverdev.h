#ifndef TCPSERVERDEV_H
#define TCPSERVERDEV_H

#include <QTcpServer>
#include <QHash>
#include <QJsonObject>
#include <QTimer>
#include "tcpsocketdev.h"

//继承QTcpServer以实现多线程TCPscoket的服务器。
class TcpServerDev : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServerDev(QObject *parent = 0,int numConnections = 10000);
    ~TcpServerDev();
    //重写设置最大连接数函数
    void setMaxPendingConnections(int numConnections);

signals:
    //发送新用户连接信息
    void connectClient(const int , const QString & ,const quint16 );
    //发送获得用户发过来的数据
    void readData(const int,const QString &, quint16, const QJsonObject &);
    //断开连接的用户信息
    void sockDisConnect(int ,QString ,quint16);
    //向scoket发送消息
    void sentData(const QJsonObject & ,const int);
    //断开特定连接，并释放资源，-1为断开所有。
    void sentDisConnect(int i);
    //设备登录成功
    void oneDeviceLoginSuccessed(int,int);
    //请求到达
    void requestSensingData(int sptr,int index,int id,const QString &pass);
    //请求返回
    void reRequestSensingData(int sptr,int index,bool re,const QJsonObject &data);
    //控制到达
    void controlCmd(int sptr,const QJsonObject &cmd);
    //控制结果返回
    void reControlCmd(int sptr,int index,bool re);

public slots:
    //断开所有连接，线程计数器清零
    void clear();

protected slots:
    //断开连接的用户信息
    void sockDisConnectSlot(int handle,const QString & ip, quint16 prot, QThread *th);
    //心跳包定时器
    void heartBeatTimerTimeoutSlot();

protected:
    //覆盖已获取多线程
    void incomingConnection(qintptr socketDescriptor);

private:
    QHash<int,TcpSocketDev *> * tcpClient;  //管理连接的map
    int maxConnections;
    QTimer *heartBeatTimer;

};

#endif // TCPSERVERDEV_H
