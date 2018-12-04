#include "datacache.h"
#include "infoOutput.h"
#include "configure.h"
#include "networkpotocol.h"
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

DataCache::DataCache(QObject *parent) : QObject(parent)
{
    updateTimer = new QTimer;

    srand(QDateTime::currentDateTime().toString("zzz").toInt());

    connect(updateTimer,SIGNAL(timeout()),
            this,SLOT(updateTimerTimeoutSlot()));
    updateTimer->start(5000);

    InfoOutput::putOutInfo("DataCache init");
}

DataCache *DataCache::getObject()
{
    static DataCache dc;
    return &dc;
}

void DataCache::setNode1Light(quint8 i, quint8 l)
{
    node1Light[i] = l;
}

void DataCache::setNode1RGBLED(quint8 g, quint8 r, quint8 b)
{
    node1Rgb[0] = g;
    node1Rgb[1] = r;
    node1Rgb[2] = b;
}

void DataCache::setNode5AlarmLight(bool sw)
{
    node5Light = sw;
}

void DataCache::setNode5AlarmBuzzer(bool sw)
{
    node5Buzzer = sw;
}

void DataCache::setNode6SteeringGear(bool sw)
{
    node6SteeringGear = sw;
}

void DataCache::setNode7Fun(bool sw)
{
    node7Fan = sw;
}

void DataCache::setNode7Motor(quint8 dir, quint16 len)
{
    if(dir == 1)
    {
        if(len + node7MotorPos >= 380)
            node7MotorPos = 380;
        else
            node7MotorPos += len;
    }
    else
    {
        if(node7MotorPos <= len)
            node7MotorPos = 0;
        else
            node7MotorPos -= len;
    }
}

void DataCache::setNode7Rec(quint8 id, bool sw)
{
    if(id == 1)
        node7Relay1 = sw;
    else if(id == 2)
        node7Relay2 = sw;
}

void DataCache::setNode8Lock(bool sw)
{
    node8Lock = sw;
}

void DataCache::updateTimerTimeoutSlot()
{
    static int updateFlag = 0;
    switch (updateFlag) {
    case 0:
        createRandSensingData();
        updateFlag++;
        break;
    case 1:
//        dispSensingData();
        submitSensingData();
        updateFlag++;
        break;
    default:
        updateFlag = 0;
        break;
    }

}

void DataCache::dispSensingData()
{
    qDebug() << "node1Light[0] = "           << node1Light[0]
             << "node1Light[1] = "           << node1Light[1]
             << "node1Light[2] = "           << node1Light[2]
             << "node1Light[3] = "           << node1Light[3]
             << "node1Rgb[0] = "           << node1Rgb[0]
             << "node1Rgb[1] = "           << node1Rgb[1]
             << "node1Rgb[2] = "           << node1Rgb[2]
             << "node1Inf = "           << node1Inf
             << " node2Te = "           << node2Te
             << " node2Hu = "           << node2Hu
             << " node2Co2 = "          << node2Co2
             << " node2Light = "        << node2Light
             << " node2UV = "           << node2UV
             << " node3MQ2 = "          << node3MQ2
             << " node3MQ5 = "          << node3MQ5
             << " node3Fire = "         << node3Fire
             << " node3Inf = "          << node3Inf
             << " node4Te = "           << node4Te
             << " node4Hu = "           << node4Hu
             << " node4Pm25 = "         << node4Pm25
             << " node5Light = "        << node5Light
             << " node5Buzzer = "       << node5Buzzer
             << " node5Opposite = "     << node5Opposite
             << " node5Reflective = "   << node5Reflective
             << " node6SteeringGear = " << node6SteeringGear
             << " node7Fan = "          << node7Fan
             << " node7MotorPos = "     << node7MotorPos
             << " node7Relay1 = "       << node7Relay1
             << " node7Relay2 = "       << node7Relay2
             << " node8Lock = "         << node8Lock;
}

void DataCache::submitSensingData()
{
    QJsonObject obj;
    obj.insert("deviceID",Configure::getObject()->getDeviceID());
    obj.insert("type","sensingData");
    obj.insert("node1Inf",node1Inf);
    QJsonArray temp;
    temp.append(node1Light[0]);
    temp.append(node1Light[1]);
    temp.append(node1Light[2]);
    temp.append(node1Light[3]);
    obj.insert("node1Light",temp);
    QJsonArray temp2;
    temp2.append(node1Rgb[0]);
    temp2.append(node1Rgb[1]);
    temp2.append(node1Rgb[2]);
    obj.insert("node1Rgb",temp2);
    obj.insert("node2Co2",node2Co2);
    obj.insert("node2Hu",node2Hu);
    obj.insert("node2Light",node2Light);
    obj.insert("node2Te",node2Te);
    obj.insert("node2UV",node2UV);
    obj.insert("node3Fire",node3Fire);
    obj.insert("node3Inf",node3Inf);
    obj.insert("node3MQ2",node3MQ2);
    obj.insert("node3MQ5",node3MQ5);
    obj.insert("node4Hu",node4Hu);
    obj.insert("node4Pm25",node4Pm25);
    obj.insert("node4Te",node4Te);
    obj.insert("node5Buzzer",node5Buzzer);
    obj.insert("node5Light",node5Light);
    obj.insert("node5Opposite",node5Opposite);
    obj.insert("node5Reflective",node5Reflective);
    obj.insert("node6SteeringGear",node6SteeringGear);
    obj.insert("node7Fan",node7Fan);
    obj.insert("node7MotorPos",node7MotorPos);
    obj.insert("node7Relay1",node7Relay1);
    obj.insert("node7Relay2",node7Relay2);
    obj.insert("node8Lock",node8Lock);
    NetWorkPotocol::getObject()->sendPacket(obj);
}

void DataCache::createRandSensingData()
{
    node1Inf = (rand()%10 > 0) ? true : false;
    node2Te = (node2Te + node4Te) / 2 + (double)(rand() % 4) / 10.0 - 0.2;
    if(node2Te < -30)
        node2Te = -30;
    else if(node2Te > 50)
        node2Te = 50;
    node2Hu = (node2Hu + node4Hu) / 2 + (rand() % 4) - 2;
    if(node2Hu < 0)
        node2Hu = 0;
    else if(node2Hu > 88)
        node2Hu = 88;
    node2Co2 += + (rand() % 200) - 100;
    if(node2Co2 < 30)
        node2Co2 = 30;
    else if(node2Co2 > 5000)
        node2Co2 = 5000;
    node2Light += rand() % 50 - 25;
    if(node2Light < 0 )
        node2Light = 0;
    else if(node2Light > 20000)
        node2Light = 20000;
    node2UV += rand() % 6 - 3;
    if(node2UV < 0)
        node2UV = 0;
    else if(node2UV > 100)
        node2UV = 100;
    node3Fire = (rand()%10 > 0) ? true : false;
    node3Inf = (rand()%10 > 0) ? true : false;
    node3MQ2 = (rand()%10 > 0) ? true : false;
    node3MQ5 = (rand()%10 > 0) ? true : false;
    node4Pm25 += rand() % 30 - 15;
    if(node4Pm25 < 0)
        node4Pm25 = 0;
    else if(node4Pm25 > 1000)
        node4Pm25 = 1000;
    node4Te = (node2Te + node4Te) / 2 + (double)(rand() % 4) / 10.0 - 0.2;
    if(node4Te < -30)
        node4Te = -30;
    else if(node4Te > 50)
        node4Te = 50;
    node4Hu = (node2Hu + node4Hu) / 2 + (rand() % 4) - 2;
    if(node4Hu < 0)
        node4Hu = 0;
    else if(node4Hu > 88)
        node4Hu = 88;
    node5Opposite = (rand()%10 > 0) ? true : false;
    node5Reflective = (rand()%10 > 0) ? true : false;
}
