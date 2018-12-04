#ifndef DATACACHE_H
#define DATACACHE_H

#include <QObject>
#include <QTimer>
#include "serialportpotocol.h"

class DataCache : public QObject
{
    Q_OBJECT
private:
    explicit DataCache(QObject *parent = 0);

public:
    static DataCache * getObject();

protected slots:
    void readPacketSlot(ReadData);
    void updateTimerTimeoutSlot();

protected:
    //更新节点一传感器数据
    void updateNode1Data(const ReadData &rdBuffer);
    //更新节点二传感器数据
    void updateNode2Data(const ReadData &rdBuffer);
    //更新节点三传感器数据
    void updateNode3Data(const ReadData &rdBuffer);
    //更新节点四传感器数据
    void updateNode4Data(const ReadData &rdBuffer);
    //更新节点五传感器数据
    void updateNode5Data(const ReadData &rdBuffer);
    //更新节点六传感器数据
    void updateNode6Data(const ReadData &rdBuffer);
    //更新节点七传感器数据
    void updateNode7Data(const ReadData &rdBuffer);
    //更新节点八传感器数据
    void updateNode8Data(const ReadData &rdBuffer);
    //打印输出传感数据
    void dispSensingData();
    void submitSensingData();

    quint8 node1Light[4];
    quint8 node1Rgb[3];
    bool node1Inf = false;              //节点1人体红外
    qreal node2Te = 0,node2Hu = 0;      //节点2温湿度
    quint16 node2Co2 = 0;               //节点2CO2
    qreal node2Light = 0;               //节点2光照
    quint16 node2UV = 0;                //节点2紫外线
    bool node3MQ2 = false;              //节点3烟雾
    bool node3MQ5 = false;              //节点3甲烷
    bool node3Fire = false;             //节点3火光
    bool node3Inf = false;              //节点3红外
    quint16 node4Pm25 = 0;              //节点4PM2.5
    qreal node4Te = 0,node4Hu = 0;      //节点4温湿度
    bool node5Light = false;
    bool node5Buzzer = false;
    bool node5Opposite = false;         //节点5对射传感器
    bool node5Reflective = false;       //节点5反射传感器
    bool node6SteeringGear = false;
    bool node7Fan = false;
    quint16 node7MotorPos = 0;
    bool node7Relay1 = false;
    bool node7Relay2 = false;
    bool node8Lock = false;

    QTimer *updateTimer;
};

#endif // DATACACHE_H
