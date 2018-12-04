#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QObject>

class Configure : public QObject
{
    Q_OBJECT
private:
    explicit Configure(QObject *parent = nullptr);

public:
    static QJsonObject getSQLConf();
    static QJsonObject getNetConf();
    static QJsonObject getStatusConf();

protected:
    static Configure *getObject();
    void initSettingFile();
    QJsonObject prGetSQLConf();
    QJsonObject prGetNetConf();
    QJsonObject prGetStatusConf();

};

#endif // CONFIGURE_H
