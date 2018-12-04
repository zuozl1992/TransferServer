#include <QDebug>
#include "datacache.h"
#include "control.h"
#include "infoOutput.h"
#include "configure.h"
#include "networkpotocol.h"
#include <QJsonArray>

DataCache::DataCache(QObject *parent) : QObject(parent)
{
    updateTimer = new QTimer;

    connect(updateTimer,SIGNAL(timeout()),
            this,SLOT(updateTimerTimeoutSlot()));
    updateTimer->start(1000);

    connect(SerialPortPotocol::getObject(),SIGNAL(readPacket(ReadData)),
            this,SLOT(readPacketSlot(ReadData)));
    InfoOutput::putOutInfo("DataCache init");
}

DataCache *DataCache::getObject()
{
    static DataCache dc;
    return &dc;
}

void DataCache::readPacketSlot(ReadData rdBuffer)
{
    //dispSensingData();
    if(rdBuffer.nodeID == 0x01)
        updateNode1Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x02)
        updateNode2Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x03)
        updateNode3Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x04)
        updateNode4Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x05)
        updateNode5Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x06)
        updateNode6Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x07)
        updateNode7Data(rdBuffer);
    else if(rdBuffer.nodeID == 0x08)
        updateNode8Data(rdBuffer);
}

void DataCache::updateTimerTimeoutSlot()
{
    static int updateFlag = 0;
    switch (updateFlag) {
    case 0:
        Control::getObject()->getNode1All();
        updateFlag++;
        break;
    case 1:
        Control::getObject()->getNode2All();
        updateFlag++;
        break;
    case 2:
        Control::getObject()->getNode3All();
        updateFlag++;
        break;
    case 3:
        Control::getObject()->getNode4All();
        updateFlag++;
        break;
    case 4:
        Control::getObject()->getNode5All();
        updateFlag++;
        break;
    case 5:
        submitSensingData();
        updateFlag++;
        break;
    default:
        updateFlag = 0;
        break;
    }
    //节点678均为执行器，不需要主动请求，操作执行器后会自动返回
//    Control::getObject()->getNode6All();
//    Control::getObject()->getNode7All();
//    Control::getObject()->getNode8All();

}

void DataCache::updateNode1Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (4 + 9))
            return;
        node1Light[0] = rdBuffer.data[0];
        node1Light[1] = rdBuffer.data[1];
        node1Light[2] = rdBuffer.data[2];
        node1Light[3] = rdBuffer.data[3];
        break;
    case 0x02:
        if(rdBuffer.len != (3 + 9))
            return;
        node1Rgb[0] = rdBuffer.data[0];
        node1Rgb[1] = rdBuffer.data[1];
        node1Rgb[2] = rdBuffer.data[2];
        break;
    case 0x03:
        if(rdBuffer.len != (1 + 9))
            return;
        node1Inf = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x04:
        if(rdBuffer.len != (8 + 9))
            return;
        node1Light[0] = rdBuffer.data[0];
        node1Light[1] = rdBuffer.data[1];
        node1Light[2] = rdBuffer.data[2];
        node1Light[3] = rdBuffer.data[3];
        node1Rgb[0] = rdBuffer.data[4];
        node1Rgb[1] = rdBuffer.data[5];
        node1Rgb[2] = rdBuffer.data[6];
        node1Inf = rdBuffer.data[7] == 0x01 ? false : true;
        break;
    default:
        break;
    }
}

void DataCache::updateNode2Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (4 + 9))
            return;
        node2Te = (qreal)rdBuffer.data[0] + (qreal)rdBuffer.data[1] / 100;
        node2Hu = (qreal)rdBuffer.data[2] + (qreal)rdBuffer.data[3] / 100;
        break;
    case 0x02:
        if(rdBuffer.len != (2 + 9))
            return;
        node2Co2 = rdBuffer.data[0] *256 + rdBuffer.data[1];
        break;
    case 0x03:
        if(rdBuffer.len != (2 + 9))
            return;
        node2Light = (rdBuffer.data[6] * 256 + rdBuffer.data[7]) * 0.6;
        break;
    case 0x04:
        if(rdBuffer.len != (2 + 9))
            return;
        node2UV = rdBuffer.data[0] * 256 + rdBuffer.data[1];
        break;
    case 0x05:
        if(rdBuffer.len != (10 + 9))
            return;
        node2Te = (qreal)rdBuffer.data[0] + (qreal)rdBuffer.data[1] / 100;
        node2Hu = (qreal)rdBuffer.data[2] + (qreal)rdBuffer.data[3] / 100;
        node2Co2 = rdBuffer.data[4] *256 + rdBuffer.data[5];
        node2Light = (rdBuffer.data[6] * 256 + rdBuffer.data[7]) * 0.6;
        node2UV = rdBuffer.data[8] * 256 + rdBuffer.data[9];
        break;
    default:
        break;
    }
}

void DataCache::updateNode3Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (1 + 9))
            return;
        node3MQ2 = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x02:
        if(rdBuffer.len != (1 + 9))
            return;
        node3MQ5 = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x03:
        if(rdBuffer.len != (1 + 9))
            return;
        node3Fire = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x04:
        if(rdBuffer.len != (1 + 9))
            return;
        node3Inf = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x05:
        if(rdBuffer.len != (4 + 9))
            return;
        node3MQ2 = rdBuffer.data[0] == 0x01 ? false : true;
        node3MQ5 = rdBuffer.data[1] == 0x01 ? false : true;
        node3Fire = rdBuffer.data[2] == 0x01 ? false : true;
        node3Inf = rdBuffer.data[3] == 0x01 ? false : true;
        break;
    default:
        break;
    }
}

void DataCache::updateNode4Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (2 + 9))
            return;
        node4Pm25 = (rdBuffer.data[0] * 256 + rdBuffer.data[1]) / 10;
        break;
    case 0x02:
        if(rdBuffer.len != (4 + 9))
            return;
        node4Te = (qreal)rdBuffer.data[0] + (qreal)rdBuffer.data[1] / 100;
        node4Hu = (qreal)rdBuffer.data[2] + (qreal)rdBuffer.data[3] / 100;
        break;
    case 0x03:
        if(rdBuffer.len != (6 + 9))
            return;
        node4Pm25 = (rdBuffer.data[0] * 256 + rdBuffer.data[1]) / 10;
        node4Te = (qreal)rdBuffer.data[2] + (qreal)rdBuffer.data[3] / 100;
        node4Hu = (qreal)rdBuffer.data[4] + (qreal)rdBuffer.data[5] / 100;
        break;
    default:
        break;
    }
}

void DataCache::updateNode5Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (1 + 9))
            return;
        node5Light = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    case 0x02:
        if(rdBuffer.len != (1 + 9))
            return;
        node5Buzzer = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    case 0x03:
        if(rdBuffer.len != (1 + 9))
            return;
        node5Opposite = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x04:
        if(rdBuffer.len != (1 + 9))
            return;
        node5Reflective = rdBuffer.data[0] == 0x01 ? false : true;
        break;
    case 0x05:
        if(rdBuffer.len != (4 + 9))
            return;
        node5Light = rdBuffer.data[0] == 0x01 ? true : false;
        node5Buzzer = rdBuffer.data[1] == 0x01 ? true : false;
        node5Opposite = rdBuffer.data[2] == 0x01 ? false : true;
        node5Reflective = rdBuffer.data[3] == 0x01 ? false : true;
        break;
    default:
        break;
    }
}

void DataCache::updateNode6Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (1 + 9))
            return;
        node6SteeringGear = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    case 0x02:
        if(rdBuffer.len != (1 + 9))
            return;
        node6SteeringGear = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    default:
        break;
    }
}

void DataCache::updateNode7Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (1 + 9))
            return;
        node7Fan = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    case 0x02:
        if(rdBuffer.len != (2 + 9))
            return;
        node7MotorPos = rdBuffer.data[0] << 8 | rdBuffer.data[1];
        break;
    case 0x03:
        if(rdBuffer.len != (2 + 9))
            return;
        node7Relay1 = rdBuffer.data[0] == 0x01 ? true : false;
        node7Relay2 = rdBuffer.data[1] == 0x01 ? true : false;
        break;
    case 0x04:
        if(rdBuffer.len != (5 + 9))
            return;
        node7Fan = rdBuffer.data[0] == 0x01 ? true : false;
        node7MotorPos = rdBuffer.data[1] << 8 | rdBuffer.data[2];
        node7Relay1 = rdBuffer.data[3] == 0x01 ? true : false;
        node7Relay2 = rdBuffer.data[4] == 0x01 ? true : false;
        break;
    default:
        break;
    }
}

void DataCache::updateNode8Data(const ReadData &rdBuffer)
{
    switch (rdBuffer.devID) {
    case 0x01:
        if(rdBuffer.len != (1 + 9))
            return;
        node8Lock = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    case 0x02:
        if(rdBuffer.len != (1 + 9))
            return;
        node8Lock = rdBuffer.data[0] == 0x01 ? true : false;
        break;
    default:
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
