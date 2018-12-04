/*
 * ClassName:   SerialPortPotocol
 * Author:      Albert
 * Data:        2018.7.19 09:08
 * Exp:         串口协议解析类，主要提供协议的解析与打包
 */
#ifndef SERIALPORTPOTOCOL_H
#define SERIALPORTPOTOCOL_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>

#define MAXCMDLEN   128     //发送命令的最大长度
#define MAXDATALEN  128     //接收数据的最大长度

//发送数据结构体
typedef struct sendData
{
    quint8 head[2];          //帧头
    quint8 reserved;         //保留
    quint8 addr[2];          //短地址
    quint8 len;              //帧长度
    quint8 nodeID;           //节点ID
    quint8 devID;            //设备ID
    quint8 cmd[MAXCMDLEN];   //命令
    quint8 foot;             //帧尾
}SendData;
//接收数据的结构体
typedef struct readData
{
    quint8 head[2];          //帧头
    quint8 reserved;         //保留
    quint8 addr[2];          //短地址
    quint8 len;              //帧长度
    quint8 nodeID;           //节点ID
    quint8 devID;            //设备ID
    quint8 data[MAXDATALEN]; //数据
    quint8 foot;             //帧尾
}ReadData;

class SerialPortPotocol : public QObject
{
    Q_OBJECT
private:
    explicit SerialPortPotocol(QObject *parent = 0);

public:
    //获取单例对象
    //@return SerialPortPotocol * 单例对象指针
    static SerialPortPotocol * getObject();
    //向串口发送数据，将sdBuffer转换成QByteArray类型
    //@return bool 操作结果
    bool sendPacket(SendData data);

signals:
    void readPacket(ReadData);

protected slots:
    //接收数据槽函数
    void readDataSlot(const QByteArray &data);

protected:
    void handleData();
    void updateNodeAddr(quint8 nodeID,quint8 addr1,quint8 addr2);
    quint8 readBuffer[MAXDATALEN + 9];  //接收数据缓冲数组
    quint16 len = 0;                    //当前接收到的数据长度
    quint8 nodeAddr[8][2];
};

#endif // SERIALPORTPOTOCOL_H
