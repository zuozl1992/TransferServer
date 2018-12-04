#include <QJsonObject>
#include "systemdev.h"
#include "infoOutput.h"
#include <QJsonObject>
#include "mysqldev.h"
#include "journal.h"
#include "encryption.h"
#include "configure.h"
#include <QDebug>

SystemDev::SystemDev(QObject *parent) : QObject(parent)
{
    server = new TcpServer;
    devServer = new TcpServerDev;
    statusInit();
    InfoOutput::putOutInfo("System start!");
    Journal::recordData(0,"System start!");
    connect(server,SIGNAL(requestSensingData(int,int,int,QString)),
            devServer,SIGNAL(requestSensingData(int,int,int,QString)));
    connect(devServer,SIGNAL(reRequestSensingData(int,int,bool,QJsonObject)),
            server,SIGNAL(reRequestSensingData(int,int,bool,QJsonObject)));
    connect(server,SIGNAL(controlCmd(int,QJsonObject)),
            devServer,SIGNAL(controlCmd(int,QJsonObject)));
    connect(devServer,SIGNAL(reControlCmd(int,int,bool)),
            server,SIGNAL(reControlCmd(int,int,bool)));
}

SystemDev::~SystemDev()
{

}

SystemDev* SystemDev::getObject()
{
    static SystemDev sd;
    return &sd;
}

void SystemDev::systemInit()
{
    Encryption::getobject();
    Journal::getobject();
    InfoOutput::getobject();
    if(mysqlInit())
    {
        InfoOutput::putOutInfo("Mysql connected successful!");
        Journal::recordData(0,"Mysql connected successful!");
    }
    else
    {
        InfoOutput::putOutErrorInfo("Mysql connected failed!");
        Journal::recordErrData("Mysql connected failed!");
    }
    if(serverInit())
    {
        InfoOutput::putOutInfo("Server listened successful!");
        Journal::recordData(0,"Server listened successful!");
    }
    else
    {
        InfoOutput::putOutErrorInfo("Server listened failed!");
        Journal::recordErrData("Server listened failed!");
    }

}

bool SystemDev::mysqlInit()
{
    QJsonObject sqlConf = Configure::getSQLConf();

    return MySqlDev::getobject()
            ->connectToDatabase(sqlConf.value("SqlHost").toString(),
                                sqlConf.value("SqlPort").toInt(),
                                sqlConf.value("SqlUserName").toString(),
                                sqlConf.value("SqlPassWord").toString(),
                                sqlConf.value("SqlDataBase").toString());
}

bool SystemDev::serverInit()
{
    QJsonObject netConf = Configure::getNetConf();
    if(!server->listen(QHostAddress::AnyIPv4,netConf.value("NTUMPort").toInt()))
    {
        return false;
    }
    if(!devServer->listen(QHostAddress::AnyIPv4,netConf.value("NTDevPort").toInt()))
    {
        return false;
    }
    return true;
}

void SystemDev::statusInit()
{
    QJsonObject statusConf = Configure::getStatusConf();
    InfoOutput::getobject()->setDisplaySwitch(statusConf.value("STInfoSwitch").toBool());
    Journal::getobject()->setDisplayLevel(statusConf.value("STJournalLevel").toInt());
    Journal::getobject()->setCatLen(statusConf.value("STJournalCatLen").toInt());
}

void SystemDev::test()
{
    mysqlTest();
}

void SystemDev::mysqlTest()
{
//    qDebug() << MySqlDev::getobject()->addNewUser("fdsf1","gsdg");
//    qDebug() << MySqlDev::getobject()->addNewSchool("内蒙古农业大学");
//    qDebug() << MySqlDev::getobject()->addNewMajor("郑州大学","物联网");
//    qDebug() << MySqlDev::getobject()->addNewClass("郑州大学","物联网","C10001");
//    qDebug() << MySqlDev::getobject()->getSchoolList();
//    qDebug() << MySqlDev::getobject()->getMajorList("郑州大学");
//    qDebug() << MySqlDev::getobject()->getClassList("郑州大学","物联网");
//    qDebug() << MySqlDev::getobject()->updateUserInfo("test0","张三","郑州大学","计算机科学与技术","16嵌入式1班","100001","fds",123);
//    qDebug() << MySqlDev::getobject()->updateUserClass("fdsf","中南大学","计算机科学与技术","C10001");
//    qDebug() << MySqlDev::getobject()->updateMajorName("郑州大学","计科","计算机科学与技术");
//    qDebug() << MySqlDev::getobject()->updateClassName("郑州大学","计算机科学与技术","C10002","C10001");
//    qDebug() << MySqlDev::getobject()->deleteUser("fdsf1");
//    qDebug() << MySqlDev::getobject()->deleteSchool("山西大学");
//    qDebug() << MySqlDev::getobject()->deleteMajor("郑州大学","计算机科学与技术");
//    qDebug() << MySqlDev::getobject()->deleteClass("内蒙古农业大学","电子科学与技术","12电科");
//    qDebug() << MySqlDev::getobject()->getUserList(
//                    "Unspecified",
//                    "Unspecified",
//                    "Unspecified"
//                    );
//    qDebug() << MySqlDev::getobject()->getUserInfo("test0");
//    qDebug() << MySqlDev::getobject()->setClassStateOfRegistration("郑州大学","计算机科学与技术","16嵌入式1班",true);
//    qDebug() << MySqlDev::getobject()->getClassStateOfRegistration(10000010);

//    qDebug() << MySqlDev::getobject()->addNewDevice("DGDsgd");
//    qDebug() << MySqlDev::getobject()->checkUserOperationJurisdiction("test0");
}
