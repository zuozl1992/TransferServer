/*
 * ClassName:   SerialPortDev
 * Author:      Albert
 * Data:        2018.7.18 14:54
 * Exp:         串口驱动类，提供串口的基本操作
 */
#ifndef SERIALPORTDEV_H
#define SERIALPORTDEV_H

#include <QObject>
#include <QSerialPort>
#include <QStringList>

class SerialPortDev : public QObject
{
    Q_OBJECT
private:
    explicit SerialPortDev(QObject *parent = 0);

public:
    //获取单例对象
    //@return SerialPortDev * 单例对象指针
    static SerialPortDev * getObject();
    //打开串口
    //@param portName 要打开的串口号
    //@return bool 操作的结果
    bool openPort(const QString &portName);
    //关闭串口
    void closePort();
    //获取当前系统可用的串口列表
    //@return QStringList 当前可用的列表
    QStringList getPortNames();
    //获取当前串口状态
    //@return bool 状态
    bool getPortStat();
    //发送数据
    //@param data 要发送的数据
    //@return bool 操作的结果
    bool sendData(const QByteArray &data);

signals:
    //接收数据信号
    //@param data 接收到的数据
    void readData(QByteArray data);

protected slots:
    //串口数据就绪槽函数
    void readyReadSlot();

private:
    QSerialPort *serial;    //串口对象指针

};

#endif // SERIALPORTDEV_H
