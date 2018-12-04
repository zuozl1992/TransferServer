#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QHash>
#include <QJsonObject>
#include "tcpsocket.h"

//继承QTcpServer以实现多线程TCPscoket的服务器。
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0,int numConnections = 10000);
    ~TcpServer();
    //重写设置最大连接数函数
    void setMaxPendingConnections(int numConnections);

signals:
    //发送新用户连接信息,内部维护列表使用
    //@param socketID 网络套接字描述符
    //@param ip IP地址
    //@param port 客户端端口
    void connectClient(const int socketID, const QString & ip,const quint16 port);
    //发送获得用户发过来的数据
    //内部未能处理的数据
    //@param socketID 网络套接字描述符
    //@param ip IP地址
    //@param port 客户端端口
    //@param data JSON数据
    void readData(const int socketID,const QString &ip, quint16 port, const QJsonObject &data);
    //断开连接的用户信息，维护列表使用
    //@param socketID 网络套接字描述符
    //@param ip IP地址
    //@param port 客户端端口
    void sockDisConnect(int socketID,QString ip,quint16 port);
    //向scoket发送消息
    //@param data 要发送的数据
    //@param socketID
    void sentData(const QJsonObject &data ,const int socketID);
    //断开特定连接，并释放资源，-1为断开所有。
    //@param i 网络套接字描述符
    void sentDisConnect(int i);
    //新用户连接信号，用于检测唯一登录
    //@param socketID 网络套接字描述符
    //@param username 成功登录的用户名
    void oneUserLoginSuccessed(const int socketID,const QString &username);
    //新管理员登录，用于检测唯一登录
    //@param socketID 网络套接字描述符
    //@param adminUsername 管理员用户名
    void oneAdminLoginSuccessed(const int socketID,const QString &adminUsername);
    //用户请求设备数据信号
    //@param sptr 网络套接字描述符，用于返回
    //@param index 请求序号
    //@param id 请求设备序号
    //@param pass 请求设备密码（MD5）加密后的密码
    void requestSensingData(int sptr,int index,int id,const QString &pass);
    //设备反馈信号
    //@param sptr 网络套接字描述符
    //@param index 请求序号
    //@param re 处理结果
    //@param data re为true时data有意义
    void reRequestSensingData(int sptr,int index,bool re,const QJsonObject &data);
    //用户请求控制信号
    //@param sptr 网络套接字描述符
    //@param cmd 控制命令
    void controlCmd(int sptr,const QJsonObject &cmd);
    //控制返回信息
    //@param sptr 网络套接字描述符
    //@param index 请求序号
    //@param re 控制结果
    void reControlCmd(int sptr,int index,bool re);

public slots:
    //断开所有连接，线程计数器清零
    void clear();

protected slots:
    //断开连接的用户信息
    void sockDisConnectSlot(int handle,const QString & ip, quint16 prot, QThread *th);

protected:
    //覆盖已获取多线程
    void incomingConnection(qintptr socketDescriptor);

private:
    QHash<int,TcpSocket *> * tcpClient; //管理连接的map
    int maxConnections;

};

#endif // TCPSERVER_H
