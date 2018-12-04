#include "widget.h"
#include "ui_widget.h"
#include "network.h"
#include "encryption.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QInputDialog>
#include <qjsondocument.h>
#include <QDateTime>
#include <QTableWidgetItem>
#include "configure.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/logo.ico"));
    this->setWindowTitle("管理终端");
    ui->stackedWidget->setCurrentIndex(0);
    ui->leUserName->setPlaceholderText("UserName");
    ui->lePassWord->setPlaceholderText("PassWord");
    ui->lePassWord->setEchoMode(QLineEdit::Password);
    ui->btnUpdateSchool->setEnabled(false);
    ui->btnUpdateMajor->setEnabled(false);
    ui->btnUpdateClass->setEnabled(false);
    ui->btnClassClose->setEnabled(false);
    ui->btnClassOpen->setEnabled(false);
    ui->leName->setEnabled(false);
    ui->leName->setPlaceholderText("姓名");
    ui->cbSchoolList4->setEnabled(false);
    ui->cbMajorList3->setEnabled(false);
    ui->cbClassList2->setEnabled(false);
    ui->leSchoolNumber->setEnabled(false);
    ui->leSchoolNumber->setPlaceholderText("学号");
    ui->lePost->setEnabled(false);
    ui->lePost->setPlaceholderText("职务");
    ui->leContactInfo->setEnabled(false);
    ui->leContactInfo->setPlaceholderText("联系方式");
    ui->btnUpdateUserInfo->setEnabled(false);
    ui->btnDeleteUser->setEnabled(false);

    ui->leAdminName->setPlaceholderText("用户名");
    ui->leAdminPass->setPlaceholderText("密码");
    ui->leAdminPass->setEchoMode(QLineEdit::Password);
    ui->leAdminNewPass->setPlaceholderText("新密码");
    ui->leAdminNewPass->setEchoMode(QLineEdit::Password);
    ui->leDevicePasswd->setPlaceholderText("密码");
    ui->leDevicePasswd->setEchoMode(QLineEdit::Password);

    ui->leDeviceID->setPlaceholderText("设备ID");
    ui->leDevicePasswd2->setPlaceholderText("设备密码");
    ui->leDevicePasswd2->setEchoMode(QLineEdit::Password);

    ui->leMyOldPass->setPlaceholderText("旧密码");
    ui->leMyOldPass->setEchoMode(QLineEdit::Password);
    ui->leMyNewPass->setPlaceholderText("新密码");
    ui->leMyNewPass->setEchoMode(QLineEdit::Password);

    ui->leMyName->setPlaceholderText("姓名");
    ui->leMySchoolNumber->setPlaceholderText("学号");
    ui->leMyPost->setPlaceholderText("职务");
    ui->leMyContactInfo->setPlaceholderText("联系方式");

    ui->leMySchoolName->setEnabled(false);
    ui->leMyMajorName->setEnabled(false);
    ui->leMyClassName->setEnabled(false);

    updateTimer = new QTimer;
    connect(updateTimer,SIGNAL(timeout()),
            this,SLOT(updateTimerTimeoutSlot()));
    updateTimer->start(1000);

    connect(NetWork::getObject(),SIGNAL(dropLine()),
            this,SLOT(dropLineSlot()));
    connect(NetWork::getObject(),SIGNAL(reLogin(bool)),
            this,SLOT(reLoginSlot(bool)));
    connect(NetWork::getObject(),SIGNAL(reAdminLogin(bool)),
            this,SLOT(reAdminLoginSlot(bool)));
    connect(NetWork::getObject(),SIGNAL(reGetSchoolList(QJsonArray)),
            this,SLOT(reGetSchoolListSlot(QJsonArray)));
    connect(NetWork::getObject(),SIGNAL(reGetMajorList(QJsonArray)),
            this,SLOT(reGetMajorListSlot(QJsonArray)));
    connect(NetWork::getObject(),SIGNAL(reGetClassList(QJsonArray)),
            this,SLOT(reGetClassListSlot(QJsonArray)));
    connect(NetWork::getObject(),SIGNAL(reGetAdminList(QJsonArray)),
            this,SLOT(reGetAdminListSlot(QJsonArray)));
    connect(NetWork::getObject(),SIGNAL(reBool(bool)),
            this,SLOT(reBoolSlot(bool)));
    connect(NetWork::getObject(),SIGNAL(reErrorInfo(QString)),
            this,SLOT(reErrorInfoSlot(QString)));
    connect(NetWork::getObject(),SIGNAL(clientConnected()),
            this,SLOT(clientConnectedSlot()));
    connect(NetWork::getObject(),SIGNAL(clientDisconnected()),
            this,SLOT(clientDisconnectedSlot()));
    connect(NetWork::getObject(),SIGNAL(newDataSend()),
            this,SLOT(newDataSendSlot()));
    connect(NetWork::getObject(),SIGNAL(reGetUserList(QJsonArray)),
            this,SLOT(reGetUserListSlot(QJsonArray)));
    connect(NetWork::getObject(),SIGNAL(reGetUserInfo(QJsonObject)),
            this,SLOT(reGetUserInfoSlot(QJsonObject)));
    connect(NetWork::getObject(),SIGNAL(reGetMyInfo(QJsonObject)),
            this,SLOT(reGetMyInfoSlot(QJsonObject)));
    connect(NetWork::getObject(),SIGNAL(reNewDeviceID(int)),
            this,SLOT(reNewDeviceIDSlot(int)));
    connect(NetWork::getObject(),SIGNAL(showData(QJsonObject)),
            this,SLOT(showDataSlot(QJsonObject)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnLogin_clicked()
{
    QString name = ui->leUserName->text();
    QString pass = ui->lePassWord->text();
    if(ui->cbAdmin->isChecked())
    {
        NetWork::getObject()->adminLogin(name,Encryption::md5(pass));
    }
    else
    {
        NetWork::getObject()->login(name,Encryption::md5(pass));
    }
}

void Widget::on_lePassWord_returnPressed()
{
    on_btnLogin_clicked();
}

void Widget::on_leUserName_returnPressed()
{
    on_btnLogin_clicked();
}

void Widget::reLoginSlot(bool re)
{
    if(re)
    {
        ui->stackedWidget->setCurrentIndex(2);
        NetWork::getObject()->getSchoolList();
        NetWork::getObject()->getMyInfo();
        ui->leMyOldPass->clear();
        ui->leMyNewPass->clear();
    }
    else
        QMessageBox::warning(this,"warning","账号或密码错误!");
}

void Widget::reAdminLoginSlot(bool re)
{
    if(re)
    {
        initAdminPage();
        ui->stackedWidget->setCurrentIndex(1);
        ui->lbRootName->setText(ui->leUserName->text());
        ui->lePassWord->clear();
    }
    else
        QMessageBox::warning(this,"warning","账号或密码错误!");
}

void Widget::dropLineSlot()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::reGetSchoolListSlot(const QJsonArray &list)
{
    ui->schoolList->clear();
    ui->cbSchoolList->clear();
    ui->cbSchoolList2->clear();
    ui->cbSchoolList3->clear();
    ui->cbSchoolList3->addItem("");
    ui->cbSchoolList4->clear();
    ui->cbSchoolList4->addItem("");
    ui->cbMySchoolList->clear();
    ui->cbMySchoolList->addItem("");
    for(int i = 0; i < list.size(); i++)
    {
        ui->schoolList->addItem(list.at(i).toString());
        ui->cbSchoolList->addItem(list.at(i).toString());
        ui->cbSchoolList2->addItem(list.at(i).toString());
        ui->cbSchoolList3->addItem(list.at(i).toString());
        ui->cbSchoolList4->addItem(list.at(i).toString());
        ui->cbMySchoolList->addItem(list.at(i).toString());
    }
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->cbSchoolList4->setCurrentText(lastSchool);
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        NetWork::getObject()->getMajorList(ui->cbSchoolList->currentText());
        ui->btnUpdateSchool->setEnabled(false);
    }
}

void Widget::reGetMajorListSlot(const QJsonArray &list)
{
    ui->majorList->clear();
    ui->cbMajorList->clear();
    ui->cbMajorList2->clear();
    ui->cbMajorList2->addItem("");
    ui->cbMajorList3->clear();
    ui->cbMajorList3->addItem("");
    ui->cbMyMajorList->clear();
    ui->cbMyMajorList->addItem("");
    for(int i = 0; i < list.size(); i++)
    {
        ui->majorList->addItem(list.at(i).toString());
        ui->cbMajorList->addItem(list.at(i).toString());
        ui->cbMajorList2->addItem(list.at(i).toString());
        ui->cbMajorList3->addItem(list.at(i).toString());
        ui->cbMyMajorList->addItem(list.at(i).toString());
    }
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->cbMajorList3->setCurrentText(lastMajor);
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        NetWork::getObject()->getClassList(ui->cbSchoolList2->currentText(),
                                           ui->cbMajorList->currentText());
        ui->btnUpdateMajor->setEnabled(false);
    }
}

void Widget::reGetClassListSlot(const QJsonArray &list)
{
    ui->classList->clear();
    ui->cbClassList->clear();
    ui->cbClassList->addItem("");
    ui->cbClassList2->clear();
    ui->cbClassList2->addItem("");
    ui->cbMyClassList->clear();
    ui->cbMyClassList->addItem("");
    for(int i = 0; i < list.size(); i++)
    {
        QString name = list.at(i).toObject().value("name").toString();
        ui->cbClassList->addItem(name);
        if(!list.at(i).toObject().value("type").toBool())
            name += "-不可注册*";
        else
        {
            ui->cbClassList2->addItem(name);
            ui->cbMyClassList->addItem(name);
        }
        ui->classList->addItem(name);

    }
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->cbClassList2->setCurrentText(lastClass);
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        ui->btnUpdateClass->setEnabled(false);
        ui->btnClassOpen->setEnabled(false);
        ui->btnClassClose->setEnabled(false);
    }
}

void Widget::reGetUserListSlot(const QJsonArray &list)
{
    ui->userList->clear();
    userList.clear();
    for(int i = 0; i < list.size(); i++)
    {
        QString name = list.at(i).toObject().value("name").toString();
        QString userName = list.at(i).toObject().value("userName").toString();
        if(name != "")
            ui->userList->addItem(name);
        else
            ui->userList->addItem("NULL");
        userList.append(userName);
    }
}

void Widget::reGetAdminListSlot(const QJsonArray &list)
{
    ui->adminList->clear();
    for(int i = 0; i < list.size(); i++)
    {
        ui->adminList->addItem(list.at(i).toString());
    }
}

void Widget::reGetUserInfoSlot(const QJsonObject &info)
{
    ui->leName->setText(info.value("name").toString());
    ui->cbSchoolList4->setCurrentText(info.value("school").toString());
    lastSchool = info.value("school").toString();
    ui->cbMajorList3->setCurrentText(info.value("major").toString());
    lastMajor = info.value("major").toString();
    ui->cbClassList2->setCurrentText(info.value("class").toString());
    lastClass = info.value("class").toString();
    ui->leSchoolNumber->setText(info.value("schoolNumber").toString());
    ui->lePost->setText(info.value("post").toString());
    ui->leContactInfo->setText(info.value("contactInfo").toString());
}

void Widget::reGetMyInfoSlot(const QJsonObject &info)
{
    ui->leMyName->setText(info.value("name").toString());
    ui->leMySchoolName->setText(info.value("school").toString());
    ui->leMyMajorName->setText(info.value("major").toString());
    ui->leMyClassName->setText(info.value("class").toString());
    ui->leMySchoolNumber->setText(info.value("schoolNumber").toString());
    ui->leMyPost->setText(info.value("post").toString());
    ui->leMyContactInfo->setText(info.value("contactInfo").toString());
}

void Widget::reBoolSlot(bool ok)
{
    if(ok)
    {
        int index0 = ui->stackedWidget->currentIndex();
        int index2;
        QMessageBox::information(this,"Infomation","操作成功");
        int index1;
        switch (index0) {
        case 1:
        {
            index1 = ui->tabWidget->currentIndex();
            switch (index1) {
            case 0:
                ui->leName->setEnabled(true);
                ui->cbSchoolList4->setEnabled(true);
                ui->cbMajorList3->setEnabled(true);
                ui->cbClassList2->setEnabled(true);
                ui->leSchoolNumber->setEnabled(true);
                ui->lePost->setEnabled(true);
                ui->leContactInfo->setEnabled(true);
                ui->btnUpdateUserInfo->setEnabled(true);
                ui->btnDeleteUser->setEnabled(true);
                NetWork::getObject()->getUserInfo(ui->lbUserName->text());
                on_btnGetUserList_clicked();
                break;
            case 1:
                index2 = ui->toolBox->currentIndex();
                switch (index2) {
                case 0:
                    NetWork::getObject()->getSchoolList();
                    break;
                case 1:
                    NetWork::getObject()->getMajorList(ui->cbSchoolList->currentText());
                    break;
                case 2:
                    NetWork::getObject()->getClassList(ui->cbSchoolList2->currentText(),
                                                       ui->cbMajorList->currentText());
                    break;
                }
                break;
            case 2:
                ui->leAdminName->clear();
                ui->leAdminPass->clear();
                ui->leAdminNewPass->clear();
                ui->leDevicePasswd->clear();
                NetWork::getObject()->getAdminList();
                break;
            }
            break;
        }
        default:
            break;
        }

    }
    else
        QMessageBox::warning(this,"Warning","操作失败");
}

void Widget::reErrorInfoSlot(const QString &data)
{
    QMessageBox::warning(this,"Warning",data);
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::reNewDeviceIDSlot(int id)
{
    QMessageBox::information(this,"information",
                             tr("申请成功，id:%1。").arg(id));
}

void Widget::newDataSendSlot()
{
    ui->lbInfo->clear();
    ui->lbInfo2->clear();
}

void Widget::clientConnectedSlot()
{
    ui->lbInfo->setText("连接成功");
    ui->lbInfo2->setText("连接成功");
}

void Widget::clientDisconnectedSlot()
{
    ui->lbInfo->setText("网络断开，正在重连");
    NetWork::getObject()->connectToServer("127.0.0.1",10001);
}

void Widget::updateTimerTimeoutSlot()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->lbTime1->setText(time);
    ui->lbTime2->setText(time);
    if(!NetWork::getObject()->getSocketState())
    {
        ui->lbInfo->setText("网络断开，正在重连");
        ui->lbInfo2->setText("网络断开，正在重连");
        NetWork::getObject()->connectToServer(Configure::getObject()->getNetWorkIpAddr()
                                              ,Configure::getObject()->getNetWorkPort());
    }
}

void Widget::showDataSlot(const QJsonObject &obj)
{
    ui->tbInfo->append(QString(QJsonDocument(obj).toJson()));
}

void Widget::initAdminPage()
{
    ui->tabWidget->setCurrentIndex(0);
    NetWork::getObject()->getSchoolList();
}

void Widget::clearUserInfoDispArea()
{
    ui->leName->setEnabled(false);
    ui->leName->clear();
    ui->cbSchoolList4->setEnabled(false);
    ui->cbMajorList3->setEnabled(false);
    ui->cbClassList2->setEnabled(false);
    ui->cbSchoolList4->setCurrentIndex(0);
    ui->cbMajorList3->setCurrentIndex(0);
    ui->cbClassList2->setCurrentIndex(0);
    ui->leSchoolNumber->setEnabled(false);
    ui->leSchoolNumber->clear();
    ui->lePost->setEnabled(false);
    ui->lePost->clear();
    ui->leContactInfo->setEnabled(false);
    ui->leContactInfo->clear();
    ui->btnUpdateUserInfo->setEnabled(false);
    ui->btnDeleteUser->setEnabled(false);
}

void Widget::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        NetWork::getObject()->getSchoolList();
    }
    else if(index == 1)
    {
        ui->toolBox->setCurrentIndex(0);
        NetWork::getObject()->getSchoolList();
    }
    else if(index == 2)
    {
        NetWork::getObject()->getAdminList();
    }
}

void Widget::on_toolBox_currentChanged(int index)
{
    if(index == 0)
    {
        NetWork::getObject()->getSchoolList();
    }
    else if(index == 1)
    {
        NetWork::getObject()->getMajorList(ui->cbSchoolList->currentText());
    }
    else if(index == 2)
    {
        NetWork::getObject()->getClassList(ui->cbSchoolList2->currentText(),
                                           ui->cbMajorList->currentText());
    }
}

void Widget::on_cbSchoolList_currentIndexChanged(int index)
{
    if( ui->toolBox->currentIndex() != 1 )
        return;
    ui->cbSchoolList2->setCurrentIndex(index);
    NetWork::getObject()->getMajorList(ui->cbSchoolList->currentText());
}

void Widget::on_cbSchoolList2_currentIndexChanged(int index)
{
    if( ui->toolBox->currentIndex() != 2 )
        return;
    ui->cbSchoolList->setCurrentIndex(index);
    NetWork::getObject()->getMajorList(ui->cbSchoolList2->currentText());
}

void Widget::on_cbMajorList_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if( ui->toolBox->currentIndex() != 2 )
        return;
    NetWork::getObject()->getClassList(ui->cbSchoolList2->currentText(),
                                       ui->cbMajorList->currentText());
}

void Widget::on_btnAddClass_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加班级"),
                                       tr("班级名称:"), QLineEdit::Normal,
                                       "", &ok);
    if (ok && !text.isEmpty())
    {
        NetWork::getObject()->addNewClass(ui->cbSchoolList2->currentText(),
                                          ui->cbMajorList->currentText(),
                                          text);
    }
}

void Widget::on_btnUpdateClass_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("修改班级名称"),
                                       tr("新班级名称:"), QLineEdit::Normal,
                                       "", &ok);
    if (ok && !text.isEmpty())
    {
        QString str = ui->classList->currentItem()->text();
        QString oldname;
        if(str.at(str.length()-1) == '*')
        {
            for(int i = 0; i < str.length()-6; i++)
                oldname.append(str.at(i));
        }
        else
        {
            oldname = str;
        }
        NetWork::getObject()->updateClass(ui->cbSchoolList2->currentText(),
                                          ui->cbMajorList->currentText(),
                                          oldname,
                                          text);
    }
}

void Widget::on_classList_itemSelectionChanged()
{
    ui->btnUpdateClass->setEnabled(true);
    QString str = ui->classList->currentItem()->text();
    if(str.at(str.length()-1) == '*')
    {
        ui->btnClassOpen->setEnabled(true);
        ui->btnClassClose->setEnabled(false);
    }
    else
    {
        ui->btnClassOpen->setEnabled(false);
        ui->btnClassClose->setEnabled(true);
    }
}

void Widget::on_btnClassOpen_clicked()
{
    QString str = ui->classList->currentItem()->text();
    QString className;
    for(int i = 0; i < str.length()-6; i++)
        className.append(str.at(i));

    NetWork::getObject()->setClassStateOfRegistration(ui->cbSchoolList2->currentText(),
                                      ui->cbMajorList->currentText(),
                                      className,true
                                      );
}

void Widget::on_btnClassClose_clicked()
{
    QString className = ui->classList->currentItem()->text();
    NetWork::getObject()->setClassStateOfRegistration(ui->cbSchoolList2->currentText(),
                                      ui->cbMajorList->currentText(),
                                      className,false
                                      );
}

void Widget::on_btnAddMajor_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加专业"),
                                       tr("专业名称:"), QLineEdit::Normal,
                                       "", &ok);
    if (ok && !text.isEmpty())
    {
        NetWork::getObject()->addNewMajor(ui->cbSchoolList->currentText(),
                                          text);
    }
}

void Widget::on_majorList_itemSelectionChanged()
{
    ui->btnUpdateMajor->setEnabled(true);
}

void Widget::on_btnUpdateMajor_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("修改专业名称"),
                                       tr("新专业名称:"), QLineEdit::Normal,
                                       "", &ok);
    if (ok && !text.isEmpty())
    {
        QString oldname = ui->majorList->currentItem()->text();
        NetWork::getObject()->updateMajor(ui->cbSchoolList->currentText(),
                                          oldname,
                                          text);
    }
}

void Widget::on_schoolList_itemSelectionChanged()
{
    ui->btnUpdateSchool->setEnabled(true);
}

void Widget::on_btnAddSchool_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加学校"),
                                       tr("学校名称:"), QLineEdit::Normal,
                                       "", &ok);
    if (ok && !text.isEmpty())
    {
        NetWork::getObject()->addNewSchool(text);
    }
}

void Widget::on_btnUpdateSchool_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("修改学校名称"),
                                       tr("新学校名称:"), QLineEdit::Normal,
                                       "", &ok);
    if (ok && !text.isEmpty())
    {
        QString oldname = ui->schoolList->currentItem()->text();
        NetWork::getObject()->updateSchool(oldname,
                                          text);
    }
}

void Widget::on_schoolList_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QString name = ui->schoolList->currentItem()->text();
    int re = QMessageBox::question(this,"删除学校",
                                   tr("确认删除%1？\n同时会删除该学校下的所有专业与班级。")
                                   .arg(name),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    switch (re) {
    case QMessageBox::Ok:
        NetWork::getObject()->deleteSchool(name);
        break;
    default:
        break;
    }
}

void Widget::on_majorList_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QString name = ui->majorList->currentItem()->text();
    int re = QMessageBox::question(this,"删除专业",
                                   tr("确认删除%1？\n同时会删除该专业下的所有班级。")
                                   .arg(name),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    switch (re) {
    case QMessageBox::Ok:
        NetWork::getObject()->deleteMajor(ui->cbSchoolList->currentText(),
                    name);
        break;
    default:
        break;
    }
}

void Widget::on_classList_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QString str = ui->classList->currentItem()->text();
    QString name;
    if(str.at(str.length()-1) == '*')
    {
        for(int i = 0; i < str.length()-6; i++)
            name.append(str.at(i));
    }
    else
    {
        name = str;
    }
    int re = QMessageBox::question(this,"删除班级",
                                   tr("确认删除%1？")
                                   .arg(name),
                                   QMessageBox::Ok | QMessageBox::Cancel);
    switch (re) {
    case QMessageBox::Ok:
        NetWork::getObject()->deleteClass(ui->cbSchoolList2->currentText(),
                             ui->cbMajorList->currentText(),
                             name);
        break;
    default:
        break;
    }
}

void Widget::on_cbSchoolList3_currentIndexChanged(int index)
{
    if(index != 0)
    {
        NetWork::getObject()->getMajorList(ui->cbSchoolList3->currentText());
    }
    else
    {
        ui->cbMajorList2->setCurrentIndex(0);
        ui->cbClassList->setCurrentIndex(0);
    }
}

void Widget::on_cbMajorList2_currentIndexChanged(int index)
{
    if(index != 0)
    {
        NetWork::getObject()->getClassList(ui->cbSchoolList3->currentText(),
                                           ui->cbMajorList2->currentText());
    }
    else
        ui->cbClassList->setCurrentIndex(0);
}

void Widget::on_cbClassList_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

void Widget::on_btnGetUserList_clicked()
{
    clearUserInfoDispArea();
    if(ui->cbSchoolList3->currentIndex() <= 0)
    {
        NetWork::getObject()->getUserList();
        return;
    }
    if(ui->cbMajorList2->currentIndex() <= 0)
    {
        NetWork::getObject()->getUserList(ui->cbSchoolList3->currentText());
        return;
    }
    if(ui->cbClassList->currentIndex() <= 0)
    {
        NetWork::getObject()->getUserList(ui->cbSchoolList3->currentText(),
                                          ui->cbMajorList2->currentText());
        return;
    }
    NetWork::getObject()->getUserList(ui->cbSchoolList3->currentText(),
                                      ui->cbMajorList2->currentText(),
                                      ui->cbClassList->currentText());
}

void Widget::on_userList_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void Widget::on_btnUpdateUserInfo_clicked()
{
    if(ui->cbClassList2->currentIndex() != 0)
    {
        NetWork::getObject()
                ->updateUserInfo(ui->lbUserName->text(),
                                 ui->leName->text(),
                                 ui->cbSchoolList4->currentText(),
                                 ui->cbMajorList3->currentText(),
                                 ui->cbClassList2->currentText(),
                                 ui->leSchoolNumber->text(),
                                 ui->lePost->text(),
                                 ui->leContactInfo->text());
    }
    else
    {
        NetWork::getObject()
                ->updateUserInfo(ui->lbUserName->text(),
                                 ui->leName->text(),
                                 ui->leSchoolNumber->text(),
                                 ui->lePost->text(),
                                 ui->leContactInfo->text());
    }
}

void Widget::on_cbSchoolList4_currentIndexChanged(int index)
{
    if(index != 0)
    {
//        NetWork::getObject()->getMajorList(ui->cbSchoolList4->currentText());
        ui->cbSchoolList3->setCurrentIndex(index);
    }
    else
    {
        ui->cbMajorList3->setCurrentIndex(0);
        ui->cbClassList2->setCurrentIndex(0);
    }
}

void Widget::on_cbMajorList3_currentIndexChanged(int index)
{
    if(index != 0)
    {
        ui->cbMajorList2->setCurrentIndex(index);
//        NetWork::getObject()->getClassList(ui->cbSchoolList4->currentText(),
//                                           ui->cbMajorList3->currentText());
    }
    else
        ui->cbClassList2->setCurrentIndex(0);
}

void Widget::on_btnLogout_clicked()
{
    NetWork::getObject()->logout();
}

void Widget::on_userList_doubleClicked(const QModelIndex &index)
{
    ui->leName->setEnabled(true);
    ui->cbSchoolList4->setEnabled(true);
    ui->cbMajorList3->setEnabled(true);
    ui->cbClassList2->setEnabled(true);
    ui->leSchoolNumber->setEnabled(true);
    ui->lePost->setEnabled(true);
    ui->leContactInfo->setEnabled(true);
    ui->btnUpdateUserInfo->setEnabled(true);
    ui->btnDeleteUser->setEnabled(true);
    ui->lbUserName->setText(userList.at(index.row()));
    NetWork::getObject()->getUserInfo(userList.at(index.row()));
}

void Widget::on_btnDeleteUser_clicked()
{
    NetWork::getObject()->deleteUser(ui->lbUserName->text());
}

void Widget::on_btnAddNewAdmin_clicked()
{
    NetWork::getObject()->addNewAdmin(
                ui->leAdminName->text(),
                Encryption::md5(ui->leAdminPass->text())
                );
}

void Widget::on_btnUpdatePasswd_clicked()
{
    NetWork::getObject()->updateAdminPassword(
                ui->lbRootName->text(),
                Encryption::md5(ui->leAdminNewPass->text())
                );
}

void Widget::on_leAdminPass_returnPressed()
{
    on_btnAddNewAdmin_clicked();
}

void Widget::on_leAdminName_returnPressed()
{
    on_btnAddNewAdmin_clicked();
}

void Widget::on_leAdminNewPass_returnPressed()
{
    on_btnUpdatePasswd_clicked();
}

void Widget::on_leDevicePasswd_returnPressed()
{
    on_btnAddNewDevice_clicked();
}

void Widget::on_btnAddNewDevice_clicked()
{
    NetWork::getObject()->addNewDevice(Encryption::md5(ui->leDevicePasswd->text()));
}

void Widget::on_btnGetSensingData_clicked()
{
    NetWork::getObject()->getSensingData(ui->leDeviceID->text().toInt(),
                                         Encryption::md5(ui->leDevicePasswd2->text()));
}

void Widget::on_btnLogout2_clicked()
{
    on_btnLogout_clicked();
}

void Widget::on_btnControl_clicked()
{
    QJsonObject obj;
    obj.insert("type","controlCmd");
    obj.insert("deviceID",ui->leDeviceID->text().toInt());
    obj.insert("passWord",Encryption::md5(ui->leDevicePasswd2->text()));
    int index = ui->cbControlType->currentIndex();
    switch (index) {
    case 0:
        obj.insert("cmdType","setNode1Light");
        obj.insert("i",2);
        obj.insert("l",50);
        break;
    case 1:
        obj.insert("cmdType","setNode1Light");
        obj.insert("i",2);
        obj.insert("l",0);
        break;
    case 2:
        obj.insert("cmdType","setNode1RGBLED");
        obj.insert("g",50);
        obj.insert("r",50);
        obj.insert("b",50);
        break;
    case 3:
        obj.insert("cmdType","setNode1RGBLED");
        obj.insert("g",0);
        obj.insert("r",0);
        obj.insert("b",0);
        break;
    case 4:
        obj.insert("cmdType","setNode5AlarmLight");
        obj.insert("sw",true);
        break;
    case 5:
        obj.insert("cmdType","setNode5AlarmLight");
        obj.insert("sw",false);
        break;
    case 6:
        obj.insert("cmdType","setNode5AlarmBuzzer");
        obj.insert("sw",true);
        break;
    case 7:
        obj.insert("cmdType","setNode5AlarmBuzzer");
        obj.insert("sw",false);
        break;
    case 8:
        obj.insert("cmdType","setNode6SteeringGear");
        obj.insert("sw",true);
        break;
    case 9:
        obj.insert("cmdType","setNode6SteeringGear");
        obj.insert("sw",false);
        break;
    case 10:
        obj.insert("cmdType","setNode7Fun");
        obj.insert("sw",true);
        break;
    case 11:
        obj.insert("cmdType","setNode7Fun");
        obj.insert("sw",false);
        break;
    case 12:
        obj.insert("cmdType","setNode7Motor");
        obj.insert("dir",1);
        obj.insert("len",380);
        break;
    case 13:
        obj.insert("cmdType","setNode7Motor");
        obj.insert("dir",2);
        obj.insert("len",380);
        break;
    case 14:
        obj.insert("cmdType","setNode7Rec");
        obj.insert("id",1);
        obj.insert("sw",true);
        break;
    case 15:
        obj.insert("cmdType","setNode7Rec");
        obj.insert("id",1);
        obj.insert("sw",false);
        break;
    case 16:
        obj.insert("cmdType","setNode7Rec");
        obj.insert("id",2);
        obj.insert("sw",true);
        break;
    case 17:
        obj.insert("cmdType","setNode7Rec");
        obj.insert("id",2);
        obj.insert("sw",false);
        break;
    case 18:
        obj.insert("cmdType","setNode8Lock");
        obj.insert("sw",true);
        break;
    case 19:
        obj.insert("cmdType","setNode8Lock");
        obj.insert("sw",false);
        break;
    default:
        break;
    }
    NetWork::getObject()->controlCmd(obj);
}

void Widget::on_btnRegister_clicked()
{
    NetWork::getObject()->addNewUser(
                ui->leUserName->text(),
                Encryption::md5(ui->lePassWord->text()));
}

void Widget::on_btnClear_clicked()
{
    ui->tbInfo->clear();
}

void Widget::on_btnChangeMyPass_clicked()
{
    NetWork::getObject()->updatePassWord1(ui->leUserName->text(),
                                          Encryption::md5(ui->leMyOldPass->text()),
                                          Encryption::md5(ui->leMyNewPass->text()));
    ui->leMyOldPass->clear();
    ui->leMyNewPass->clear();
}

void Widget::on_cbMySchoolList_currentIndexChanged(int index)
{
    if(index == 0)
        return;
    NetWork::getObject()->getMajorList(ui->cbMySchoolList->currentText());
}

void Widget::on_cbMyMajorList_currentIndexChanged(int index)
{
    if(index == 0)
        return;
    NetWork::getObject()->getClassList(ui->cbMySchoolList->currentText(),
                                       ui->cbMyMajorList->currentText());
}

void Widget::on_btnChangeMyInfo_clicked()
{
    if(ui->cbMyClassList->currentIndex() != 0)
    {
        NetWork::getObject()
                ->updateMyInfo(ui->leUserName->text(),
                                 ui->leMyName->text(),
                                 ui->cbMySchoolList->currentText(),
                                 ui->cbMyMajorList->currentText(),
                                 ui->cbMyClassList->currentText(),
                                 ui->leMySchoolNumber->text(),
                                 ui->leMyPost->text(),
                                 ui->leMyContactInfo->text());
    }
    else
    {
        NetWork::getObject()
                ->updateMyInfo(ui->leUserName->text(),
                               ui->leMyName->text(),
                               ui->leMySchoolNumber->text(),
                               ui->leMyPost->text(),
                               ui->leMyContactInfo->text());
    }
    NetWork::getObject()->getMyInfo();
}
