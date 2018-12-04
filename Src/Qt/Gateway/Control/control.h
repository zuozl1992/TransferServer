#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include "serialportpotocol.h"

class Control : public QObject
{
    Q_OBJECT
private:
    explicit Control(QObject *parent = 0);

public:
    static Control *getObject();
    //设置节点1高亮LED
    //@param i 行号 0~3
    //@param l 亮度 0~255
    //@return bool 操作结果
    bool setNode1Light(quint8 i,quint8 l);
    //设置节点1炫彩LED
    //@param x,y 坐标
    //@param g,r,b 颜色
    //@param l 长度
    //@return bool 操作结果
    bool setNode1RGBLED(quint8 g,quint8 r,quint8 b);
    //请求节点1人体红外传感数据
    //@return bool 操作结果
    bool getNode1Inf();
    //请求节点1所有数据
    bool getNode1All();
    //获取节点2温湿度数据
    //@return bool 操作结果
    bool getNode2TeHu();
    //获取节点2二氧化碳数据
    //@return bool 操作结果
    bool getNode2Co2();
    //获取节点2光照强度数据
    //@return bool 操作结果
    bool getNode2Light();
    //获取节点2紫外线数据
    //@return bool 操作结果
    bool getNode2UV();
    //获取节点2所有数据
    //@return bool 操作结果
    bool getNode2All();
    //获取节点3烟雾数据
    //@return bool 操作结果
    bool getNode3MQ2();
    //获取节点3甲烷数据
    //@return bool 操作结果
    bool getNode3MQ5();
    //获取节点3火光数据
    //@return bool 操作结果
    bool getNode3Fire();
    //获取节点3人体红外数据
    //@return bool 操作结果
    bool getNode3Inf();
    //获取节点3所有数据
    //@return bool 操作结果
    bool getNode3All();
    //获取节点4PM2.5数据
    //@return bool 操作结果
    bool getNode4PM();
    //获取节点4温湿度
    //@return bool 操作结果
    bool getNode4TeHu();
    //获取节点4所有数据
    //@return bool 操作结果
    bool getNode4All();
    //设置节点5报警灯
    //@param sw 报警灯开关
    //@return bool 操作结果
    bool setNode5AlarmLight(bool sw);
    //设置节点5报警器
    //@param sw 报警器开关
    //@return bool 操作结果
    bool setNode5AlarmBuzzer(bool sw);
    //获取节点5对射传感器数据
    //@return bool 操作结果
    bool getNode5Opposite();
    //获取节点5反射传感器数据
    //@return bool 操作结果
    bool getNode5Reflective();
    //获取节点5所有数据
    //@return bool 操作结果
    bool getNode5All();
    //设置节点6舵机
    //@param sw 舵机转动方向
    //@return bool 操作结果
    bool setNode6SteeringGear(bool sw);
    //获取节点6所有数据
    //@return bool 操作结果
    bool getNode6All();
    //设置节点7风扇
    //@param sw 风扇开关
    //@return bool 操作结果
    bool setNode7Fun(bool sw);
    //设置步进电机运动方向
    //@param dir 方向 0x01 向前，0x02 向后
    //@param len 运动的长度 0~380
    bool setNode7Motor(quint8 dir, quint16 len);
    //设置节点7继电器
    //@param id 继电器序号
    //@param sw 继电器开关
    //@return bool 操作结果
    bool setNode7Rec(quint8 id,bool sw);
    //获取节点7所有数据
    //@return bool 操作结果
    bool getNode7All();
    //设置节点8电磁锁
    //@param sw 电磁锁开关
    //@return bool 操作结果
    bool setNode8Lock(bool sw);
    //获取节点8所有数据
    //@return bool 操作结果
    bool getNode8All();

protected:
    void handleFixedData(SendData &sendData);
};

#endif // CONTROL_H
