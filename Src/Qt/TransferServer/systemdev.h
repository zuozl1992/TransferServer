/*
 * ClassName：SystemDev
 * Description：系统运行
 *
 * Author：Albert
 * Version：V0.1
 * Date：2017.08.08
 * Version Description：
 *          【2017.08.08】 V0.1
 *              1.创建该类
 *              2.将类构造函数私有化，不允许创建对象。
 *              3.添加系统初始化静态函数，在该函数中获取单例的成员对象，并且启动Tcp服务。
 */
#ifndef SYSTEMDEV_H
#define SYSTEMDEV_H

#include <QObject>
#include "tcpserver.h"
#include "tcpserverdev.h"

class SystemDev : public QObject
{
    Q_OBJECT
private:
    explicit SystemDev(QObject *parent = 0);

public:
    ~SystemDev();
    //系统初始化
    static SystemDev * getObject();
    void systemInit();

protected:
    bool mysqlInit();
    bool serverInit();
    void statusInit();

    void test();
    void mysqlTest();
    TcpServer *server;
    TcpServerDev *devServer;

};

#endif // SYSTEMDEV_H
