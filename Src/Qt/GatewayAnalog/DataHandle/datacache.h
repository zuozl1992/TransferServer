#ifndef DATACACHE_H
#define DATACACHE_H

#include <QObject>
#include <QTimer>

class DataCache : public QObject
{
    Q_OBJECT
private:
    explicit DataCache(QObject *parent = 0);

public:
    static DataCache * getObject();
    //设置节点1高亮LED
    //@param i 行号 0~3
    //@param l 亮度 0~255
    void setNode1Light(quint8 i,quint8 l);
    //设置节点1炫彩LED
    //@param x,y 坐标
    //@param g,r,b 颜色
    //@param l 长度
    void setNode1RGBLED(quint8 g,quint8 r,quint8 b);
    //设置节点5报警灯
    //@param sw 报警灯开关
    void setNode5AlarmLight(bool sw);
    //设置节点5报警器
    //@param sw 报警器开关
    void setNode5AlarmBuzzer(bool sw);
    //设置节点6舵机
    //@param sw 舵机转动方向
    void setNode6SteeringGear(bool sw);
    //设置节点7风扇
    //@param sw 风扇开关
    void setNode7Fun(bool sw);
    //设置步进电机运动方向
    //@param dir 方向 0x01 向前，0x02 向后
    //@param len 运动的长度 0~380
    void setNode7Motor(quint8 dir, quint16 len);
    //设置节点7继电器
    //@param id 继电器序号
    //@param sw 继电器开关
    void setNode7Rec(quint8 id,bool sw);
    //设置节点8电磁锁
    //@param sw 电磁锁开关
    void setNode8Lock(bool sw);

protected slots:
    void updateTimerTimeoutSlot();

protected:
    //打印输出传感数据
    void dispSensingData();
    void submitSensingData();
    void createRandSensingData();

    quint8 node1Light[4];
    quint8 node1Rgb[3];
    bool node1Inf = false;              //节点1人体红外
    qreal node2Te = 25.5,node2Hu = 55;  //节点2温湿度
    qint16 node2Co2 = 126;             //节点2CO2
    qreal node2Light = 347;             //节点2光照
    qint16 node2UV = 3;                //节点2紫外线
    bool node3MQ2 = false;              //节点3烟雾
    bool node3MQ5 = false;              //节点3甲烷
    bool node3Fire = false;             //节点3火光
    bool node3Inf = false;              //节点3红外
    qint16 node4Pm25 = 26;             //节点4PM2.5
    qreal node4Te = 25.7,node4Hu = 58;  //节点4温湿度
    bool node5Light = false;
    bool node5Buzzer = false;
    bool node5Opposite = false;         //节点5对射传感器
    bool node5Reflective = false;       //节点5反射传感器
    bool node6SteeringGear = false;
    bool node7Fan = false;
    qint16 node7MotorPos = 32;
    bool node7Relay1 = false;
    bool node7Relay2 = false;
    bool node8Lock = false;

    QTimer *updateTimer;
};

#endif // DATACACHE_H
