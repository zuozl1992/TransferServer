#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QList>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btnLogin_clicked();

    void on_lePassWord_returnPressed();

    void on_leUserName_returnPressed();

    void reLoginSlot(bool re);
    void reAdminLoginSlot(bool re);
    void dropLineSlot();
    void reGetSchoolListSlot(const QJsonArray &list);
    void reGetMajorListSlot(const QJsonArray &list);
    void reGetClassListSlot(const QJsonArray &list);
    void reGetUserListSlot(const QJsonArray &list);
    void reGetAdminListSlot(const QJsonArray &list);
    void reGetUserInfoSlot(const QJsonObject &info);
    void reGetMyInfoSlot(const QJsonObject &info);
    void reBoolSlot(bool ok);
    void reErrorInfoSlot(const QString &data);
    void reNewDeviceIDSlot(int id);
    void newDataSendSlot();
    void clientConnectedSlot();
    void clientDisconnectedSlot();
    void updateTimerTimeoutSlot();
    void showDataSlot(const QJsonObject &obj);

    void on_tabWidget_currentChanged(int index);

    void on_toolBox_currentChanged(int index);

    void on_cbSchoolList_currentIndexChanged(int index);

    void on_cbSchoolList2_currentIndexChanged(int index);

    void on_cbMajorList_currentIndexChanged(int index);

    void on_btnAddClass_clicked();

    void on_btnUpdateClass_clicked();

    void on_classList_itemSelectionChanged();

    void on_btnClassOpen_clicked();

    void on_btnClassClose_clicked();

    void on_btnAddMajor_clicked();

    void on_majorList_itemSelectionChanged();

    void on_btnUpdateMajor_clicked();

    void on_schoolList_itemSelectionChanged();

    void on_btnAddSchool_clicked();

    void on_btnUpdateSchool_clicked();

    void on_schoolList_doubleClicked(const QModelIndex &index);

    void on_majorList_doubleClicked(const QModelIndex &index);

    void on_classList_doubleClicked(const QModelIndex &index);

    void on_cbSchoolList3_currentIndexChanged(int index);

    void on_cbMajorList2_currentIndexChanged(int index);

    void on_cbClassList_currentIndexChanged(int index);

    void on_btnGetUserList_clicked();

    void on_userList_clicked(const QModelIndex &index);

    void on_btnUpdateUserInfo_clicked();

    void on_cbSchoolList4_currentIndexChanged(int index);

    void on_cbMajorList3_currentIndexChanged(int index);

    void on_btnLogout_clicked();

    void on_userList_doubleClicked(const QModelIndex &index);

    void on_btnDeleteUser_clicked();

    void on_btnAddNewAdmin_clicked();

    void on_btnUpdatePasswd_clicked();

    void on_leAdminPass_returnPressed();

    void on_leAdminName_returnPressed();

    void on_leAdminNewPass_returnPressed();

    void on_leDevicePasswd_returnPressed();

    void on_btnAddNewDevice_clicked();

    void on_btnGetSensingData_clicked();

    void on_btnLogout2_clicked();

    void on_btnControl_clicked();

    void on_btnRegister_clicked();

    void on_btnClear_clicked();

    void on_btnChangeMyPass_clicked();

    void on_cbMySchoolList_currentIndexChanged(int index);

    void on_cbMyMajorList_currentIndexChanged(int index);


    void on_btnChangeMyInfo_clicked();

private:
    void initAdminPage();
    void clearUserInfoDispArea();

    Ui::Widget *ui;
    QTimer *updateTimer;
    QList<QString> userList;
    QString lastSchool;
    QString lastMajor;
    QString lastClass;
};

#endif // WIDGET_H
