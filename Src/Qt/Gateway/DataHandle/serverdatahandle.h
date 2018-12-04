#ifndef SERVERDATAHANDLE_H
#define SERVERDATAHANDLE_H

#include <QObject>

class ServerDataHandle : public QObject
{
    Q_OBJECT
private:
    explicit ServerDataHandle(QObject *parent = 0);

public:
    static ServerDataHandle * getObject();

protected slots:
    void readPacketSlot(const QJsonObject &data);

protected:
    void handleControlCmdData(const QJsonObject &data);
};

#endif // SERVERDATAHANDLE_H
