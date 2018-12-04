#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

class QTcpSocket;

class NetWork : public QObject
{
    Q_OBJECT
private:
    explicit NetWork(QObject *parent = nullptr);

public:
    static NetWork *getObject();
    void connectToServer(QString ip,quint16 port);
    bool getSocketState();
    void login(QString userName,QString passWord);
    void logout();
    void adminLogin(QString userName,QString passWord);
    void getSchoolList();
    void getMajorList(const QString &schoolName);
    void getClassList(const QString &schoolName,const QString &majorName);
    void getUserList(const QString &schoolName,const QString &majorName,
                     const QString &className);
    void getUserList(const QString &schoolName,const QString &majorName);
    void getUserList(const QString &schoolName);
    void getUserList();
    void getAdminList();
    void getUserInfo(const QString &userName);
    void getMyInfo();
    void addNewClass(const QString &schoolName,const QString &majorName,
                     const QString &className);
    void addNewMajor(const QString &schoolName,const QString &majorName);
    void addNewSchool(const QString &schoolName);
    void addNewDevice(const QString &cDevicePasswd);
    void addNewAdmin(const QString &cUserName,const QString &cPasswd);
    void updateClass(const QString &schoolName,const QString &majorName,
                     const QString &oldName,const QString &newName);
    void updateMajor(const QString &schoolName,
                     const QString &oldName,const QString &newName);
    void updateSchool(const QString &oldName,const QString &newName);
    void updateUserInfo(const QString &cUserName,
                        const QString &cName,
                        const QString &cSchool,
                        const QString &cMajor,
                        const QString &cClass,
                        const QString &cSchoolNumber,
                        const QString &cPost,
                        const QString &cContactInfo);
    void updateUserInfo(const QString &cUserName,
                        const QString &cName,
                        const QString &cSchoolNumber,
                        const QString &cPost,
                        const QString &cContactInfo);
    void updateMyInfo(const QString &cUserName,
                        const QString &cName,
                        const QString &cSchool,
                        const QString &cMajor,
                        const QString &cClass,
                        const QString &cSchoolNumber,
                        const QString &cPost,
                        const QString &cContactInfo);
    void updateMyInfo(const QString &cUserName,
                        const QString &cName,
                        const QString &cSchoolNumber,
                        const QString &cPost,
                        const QString &cContactInfo);
    void updateAdminPassword(const QString &cUserName, const QString &cPasswd);
    void updatePassWord1(const QString &cUserName,const QString &coldPasswd, const QString &cPasswd);
    void updatePassWord2(const QString &cUserName, const QString &cPasswd);
    void setClassStateOfRegistration(const QString &schoolName,const QString &majorName,
                                     const QString &className,bool state);
    void deleteClass(const QString &schoolName,const QString &majorName,
                     const QString &className);
    void deleteMajor(const QString &schoolName,const QString &majorName);
    void deleteSchool(const QString &schoolName);
    void deleteUser(const QString &userName);
    void getSensingData(int id,const QString &password);

    void controlCmd(QJsonObject obj);
    void addNewUser(const QString &userName,
                  const QString &password);

signals:
    void reLogin(bool re);
    void reAdminLogin(bool re);
    void dropLine();
    void reGetSchoolList(const QJsonArray &list);
    void reGetMajorList(const QJsonArray &list);
    void reGetClassList(const QJsonArray &list);
    void reGetUserList(const QJsonArray &list);
    void reGetAdminList(const QJsonArray &list);
    void reGetUserInfo(const QJsonObject &info);
    void reGetMyInfo(const QJsonObject &info);
    void reBool(bool ok);
    void reErrorInfo(const QString &data);
    void reNewDeviceID(int id);
    void newDataSend();
    void clientConnected();
    void clientDisconnected();
    void showData(const QJsonObject &data);

protected slots:
    void readyReadSlot();

protected:
    void sendJson(QJsonObject obj);

    void handleData(QByteArray data);

    QTcpSocket *socket;
    QString key;
    QString adminKey;
    int requestIndex = 0;
    QByteArray readBuffer;
    quint8 lastChar = ' ';
    qint32 braNum = 0;
};

#endif // NETWORK_H
