/*
 * ClassName:   NetWorkDev
 * Author:      Albert
 * Data:        2018.7.21 15:20
 * Exp:         网络驱动类，提供网络基本服务
 */
#ifndef NETWORKDEV_H
#define NETWORKDEV_H

#include <QObject>
#include <QTcpSocket>

class NetWorkDev : public QObject
{
    Q_OBJECT
private:
    explicit NetWorkDev(QObject *parent = 0);

public:
    //获取单例对象
    //@return NetWorkDev * 单例对象
    static NetWorkDev * getObject();
    void openNet(const QString &ipAddr, quint16 port);
    bool getNetStat();
    void closeNet();
    bool sendData(const QByteArray &data);

protected slots:
    void socketReadyReadSlot();
    void socketConnectedSlot();
    void socketDisconnectedSlot();

signals:
    void readData(QByteArray data);

protected:
    QTcpSocket *socket;
    bool isConnected = false;
};

#endif // NETWORKDEV_H
