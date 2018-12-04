/*
 * ClassName:   NetWorkPotocol
 * Author:      Albert
 * Data:        2018.7.22 10:05
 * Exp:         网络协议类，提供网络协议解析与封装
 */
#ifndef NETWORKPOTOCOL_H
#define NETWORKPOTOCOL_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>

class NetWorkPotocol : public QObject
{
    Q_OBJECT
private:
    explicit NetWorkPotocol(QObject *parent = 0);

public:
    //获取单例对象
    //@return NetWorkPotocol * 单例对象指针
    static NetWorkPotocol * getObject();
    bool sendPacket(const QJsonObject &data);

signals:
    void readPacket(QJsonObject);

protected slots:
    //接收数据槽函数
    void readDataSlot(const QByteArray &data);
    void heartBeatTimerTimeoutSlot();

protected:
    void cacheReadBuffer();

    QByteArray readBuffer;
    QString key;
    int braNum = 0;
    quint8 lastChar = ' ';
    QTimer *heartBeatTimer;
    bool isReceivedHeart = false;
};

#endif // NETWORKPOTOCOL_H
