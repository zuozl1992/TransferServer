#include "control.h"
#include "infoOutput.h"

Control::Control(QObject *parent) : QObject(parent)
{
    InfoOutput::putOutInfo("Control init");
}

Control *Control::getObject()
{
    static Control c;
    return &c;
}

bool Control::setNode1Light(quint8 i, quint8 l)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x01;
    sdBuffer.devID = 0x01;
    sdBuffer.cmd[0] = i;
    sdBuffer.cmd[1] = l;
    sdBuffer.len = 9 + 2;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode1RGBLED(quint8 g, quint8 r, quint8 b)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x01;
    sdBuffer.devID = 0x02;
    sdBuffer.cmd[0] = g;
    sdBuffer.cmd[1] = r;
    sdBuffer.cmd[2] = b;
    sdBuffer.len = 9 + 3;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode1Inf()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x01;
    sdBuffer.devID = 0x03;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode1All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x01;
    sdBuffer.devID = 0x04;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode2TeHu()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x02;
    sdBuffer.devID = 0x01;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode2Co2()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x02;
    sdBuffer.devID = 0x02;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode2Light()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x02;
    sdBuffer.devID = 0x03;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode2UV()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x02;
    sdBuffer.devID = 0x04;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode2All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x02;
    sdBuffer.devID = 0x05;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode3MQ2()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x03;
    sdBuffer.devID = 0x01;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode3MQ5()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x03;
    sdBuffer.devID = 0x02;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode3Fire()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x03;
    sdBuffer.devID = 0x03;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode3Inf()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x03;
    sdBuffer.devID = 0x04;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode3All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x03;
    sdBuffer.devID = 0x05;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode4PM()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x04;
    sdBuffer.devID = 0x01;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode4TeHu()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x04;
    sdBuffer.devID = 0x02;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode4All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x04;
    sdBuffer.devID = 0x03;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode5AlarmLight(bool sw)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x05;
    sdBuffer.devID = 0x01;
    sdBuffer.cmd[0] = sw ? 0x01 : 0x00;
    sdBuffer.len = 9 + 1;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode5AlarmBuzzer(bool sw)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x05;
    sdBuffer.devID = 0x02;
    sdBuffer.cmd[0] = sw ? 0x01 : 0x00;
    sdBuffer.len = 9 + 1;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode5Opposite()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x05;
    sdBuffer.devID = 0x03;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode5Reflective()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x05;
    sdBuffer.devID = 0x04;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode5All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x05;
    sdBuffer.devID = 0x05;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode6SteeringGear(bool sw)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x06;
    sdBuffer.devID = 0x01;
    sdBuffer.cmd[0] = sw ? 0x01 : 0x00;
    sdBuffer.len = 9 + 1;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode6All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x06;
    sdBuffer.devID = 0x02;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode7Fun(bool sw)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x07;
    sdBuffer.devID = 0x01;
    sdBuffer.cmd[0] = sw ? 0x01 : 0x00;
    sdBuffer.len = 9 + 1;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode7Motor(quint8 dir, quint16 len)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x07;
    sdBuffer.devID = 0x02;
    sdBuffer.cmd[0] =dir;
    sdBuffer.cmd[1] = len >> 8;
    sdBuffer.cmd[2] = len;
    sdBuffer.len = 9 + 3;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode7Rec(quint8 id, bool sw)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x07;
    sdBuffer.devID = 0x03;
    sdBuffer.cmd[0]= id;
    sdBuffer.cmd[1] = sw ? 0x01 : 0x00;
    sdBuffer.len = 9 + 2;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode7All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x07;
    sdBuffer.devID = 0x04;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::setNode8Lock(bool sw)
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x08;
    sdBuffer.devID = 0x01;
    sdBuffer.cmd[0] = sw ? 0x01 : 0x00;
    sdBuffer.len = 9 + 1;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

bool Control::getNode8All()
{
    SendData sdBuffer;
    handleFixedData(sdBuffer);
    sdBuffer.nodeID = 0x08;
    sdBuffer.devID = 0x02;
    sdBuffer.len = 9;
    return SerialPortPotocol::getObject()->sendPacket(sdBuffer);
}

void Control::handleFixedData(SendData &sendData)
{
    sendData.head[0] = 0xFE;
    sendData.head[1] = 0xFE;
    sendData.reserved = 0x00;
    sendData.addr[0] = 0xFF;
    sendData.addr[1] = 0xFF;
    sendData.foot = 0xFF;
}
