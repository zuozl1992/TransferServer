#include <QSerialPortInfo>
#include "serialportdev.h"
#include "infoOutput.h"

SerialPortDev::SerialPortDev(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort;
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(serial,SIGNAL(readyRead()),
            this,SLOT(readyReadSlot()));
    InfoOutput::putOutInfo("SerialPortDev init");
}

SerialPortDev *SerialPortDev::getObject()
{
    static SerialPortDev sd;
    return &sd;
}

bool SerialPortDev::openPort(const QString &portName)
{
    if(!serial)
        return false;
    closePort();
    serial->setPortName(portName);
    bool ok = serial->open(QIODevice::ReadWrite);
    if(ok)
        InfoOutput::putOutInfo(tr("SerialPortDev open %1 successful").arg(portName));
    else
        InfoOutput::putOutInfo(tr("SerialPortDev open %1 failed").arg(portName));
    return ok;
}

void SerialPortDev::closePort()
{
    if(!serial)
        return;
    serial->close();
}

bool SerialPortDev::sendData(const QByteArray &data)
{
    if(!serial)
        return false;
    if(!serial->isOpen())
        return false;
    qint64 re = serial->write(data);
    return re==-1 ? false : true;
}

QStringList SerialPortDev::getPortNames()
{
    QStringList list;
    foreach (const QSerialPortInfo &info,
             QSerialPortInfo::availablePorts()) {
        list.append(info.portName());
    }
    return list;
}

bool SerialPortDev::getPortStat()
{
    if(!serial)
        return false;
    return serial->isOpen();
}

void SerialPortDev::readyReadSlot()
{
    QByteArray data;
    data.append(serial->readAll());
    emit readData(data);
}
