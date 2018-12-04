#include "tcpserverdev.h"
#include "threadhandledev.h"
#include "infoOutput.h"

TcpServerDev::TcpServerDev(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
     tcpClient = new  QHash<int,TcpSocketDev *>;
     //设备心跳包定时器
     heartBeatTimer = new QTimer;
     connect(heartBeatTimer,SIGNAL(timeout()),
             this,SLOT(heartBeatTimerTimeoutSlot()));
     heartBeatTimer->start(5000);

     setMaxPendingConnections(numConnections);
}

TcpServerDev::~TcpServerDev()
{
    emit this->sentDisConnect(-1);
    delete tcpClient;
}

void TcpServerDev::setMaxPendingConnections(int numConnections)
{
    //调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->QTcpServer::setMaxPendingConnections(numConnections);
    this->maxConnections = numConnections;
}

//多线程必须在此函数里捕获新连接
void TcpServerDev::incomingConnection(qintptr socketDescriptor)
{
    //继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    if (tcpClient->size() > maxPendingConnections())
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    auto th = ThreadHandleDev::getClass().getThread();
    auto tcpTemp = new TcpSocketDev(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();
    //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是靠的他
    connect(tcpTemp,SIGNAL(sockDisConnect(int,QString,quint16,QThread*)),
            this,SLOT(sockDisConnectSlot(int,QString,quint16,QThread*)));
    //接受数据信号绑定
    connect(tcpTemp,SIGNAL(readData(int,QString,quint16,QJsonObject)),
            this,SIGNAL(readData(int,QString,quint16,QJsonObject)));
    //传感数据返回
    connect(tcpTemp,SIGNAL(reRequestSensingData(int,int,bool,QJsonObject)),
            this,SIGNAL(reRequestSensingData(int,int,bool,QJsonObject)));
    //请求到达
    connect(this,SIGNAL(requestSensingData(int,int,int,QString)),
            tcpTemp,SLOT(requestSensingDataSlot(int,int,int,QString)));
    //断开信号
    connect(this,SIGNAL(sentDisConnect(int)),
            tcpTemp,SLOT(disConTcp(int)));
    //设备成功登录
    connect(tcpTemp,SIGNAL(oneDeviceLoginSucceed(int,int)),
            this,SIGNAL(oneDeviceLoginSuccessed(int,int)));
    //账号掉线
    connect(this,SIGNAL(oneDeviceLoginSuccessed(int,int)),
            tcpTemp,SLOT(serverOneDeviceLoginSucceedSlot(int,int)));
    //接受数据
    connect(this,SIGNAL(sentData(QJsonObject,int)),
            tcpTemp,SLOT(sentData(QJsonObject,int)));
    //控制到达
    connect(this,SIGNAL(controlCmd(int,QJsonObject)),
            tcpTemp,SLOT(controlCmdSlot(int,QJsonObject)));
    //控制返回
    connect(tcpTemp,SIGNAL(reControlCmd(int,int,bool)),
            this,SIGNAL(reControlCmd(int,int,bool)));
    //把tcp类移动到新的线程，从线程管理类中获取
    tcpTemp->moveToThread(th);
    //插入到连接信息中
    tcpClient->insert(socketDescriptor,tcpTemp);
    //发送新客户端连接信号
    emit connectClient(socketDescriptor,ip,port);
}

void TcpServerDev::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    //连接管理中移除断开连接的socket
    tcpClient->remove(handle);
    //告诉线程管理类那个线程里的连接断开了
    ThreadHandleDev::getClass().removeThread(th);
    emit sockDisConnect(handle,ip,prot);
}

void TcpServerDev::heartBeatTimerTimeoutSlot()
{
    QJsonObject obj;
    obj.insert("type","heartBeat");
    emit sentData(obj,-1);
}

void TcpServerDev::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandleDev::getClass().clear();
    tcpClient->clear();
}
