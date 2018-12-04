#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QObject>

class Configure : public QObject
{
    Q_OBJECT
private:
    explicit Configure(QObject *parent = 0);

public:
    static Configure *getObject();
    QString getNetWorkIpAddr();
    qint16 getNetWorkPort();

protected:
    void initSettingFile();

};

#endif // CONFIGURE_H
