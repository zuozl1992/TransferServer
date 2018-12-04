/*
 * ClassName：MySqlDev
 * Description：数据库操作类
 *
 * Author：Albert
 * Version：V0.2
 * Date：2018.09.21
 * Version Description：
 *          【2018.09.21】 V0.2
 *              1.设置班级注册状态
 *              2.修改信息状态校验
 *              3.修改用户除学校专业班级外的信息
 *              4.管理员注册
 *              5.管理员修改密码
 *              6.管理员登陆
 *              7.用户登录
 *              8.设备登陆
 *              9.添加设备
 *          【2018.09.20】
 *              1.查询用户列表
 *              2.查询用户信息
 *              3.修改用户密码
 *              4.修改学校名称
 *              5.修改专业名称
 *              6.修改班级名称
 *              7.删除用户
 *              8.删除学校
 *              9.删除专业
 *              10.删除班级
 *          【2018.09.19】
 *              1.重构该类
 *              2.添加用户
 *              3.添加学校
 *              4.添加专业
 *              5.添加班级
 *              6.获取专业列表
 *              7.获取学校列表
 *              8.获取班级列表
 *          【2017.07.19】 V0.1
 *              1.创建该类。
 *              2.更改该类为单例类。
 *              3.添加连接数据库函数，能够根据用户名、密码、host、端口连接数据库。
 *              4.添加检查数据库状态的函数，可以返回当前的数据库连接状态。
 *              5.添加添加用户的函数。
 *              6.添加用户登陆的函数。
 */
#ifndef MYSQLDEV_H
#define MYSQLDEV_H

#include <QObject>

class QSqlDatabase;
class QSqlQuery;
class QTimer;
class QStringList;
class QJsonObject;

class MySqlDev : public QObject
{
    Q_OBJECT
private:
    explicit MySqlDev(QObject *parent = nullptr);

public:
    ~MySqlDev();
    //[static]获取单例对象
    //@return MySqlDev *单例对象
    static MySqlDev *getobject();
    //添加管理员到数据库
    //@param cUserName 用户名
    //@param cPasswd 密码
    //@return bool 操作结果
    bool addNewAdmin(const QString &cUserName,const QString &cPasswd);
    //添加班级
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cName 班级名称
    //@return bool 操作结果
    bool addNewClass(const QString &cSchoolName,const QString &cMajorName,const QString &cName);
    //添加新设备
    //@param cDevicePasswd 设备密码
    //@return qint32 设备序号
    qint32 addNewDevice(const QString &cDevicePasswd);
    //添加专业
    //@param cSchoolName 学校名称
    //@param cName 专业名称
    //@return bool 操作结果
    bool addNewMajor(const QString &cSchoolName,const QString &cName);
    //添加学校
    //@param cName 学校名称
    //@return 操作结果
    bool addNewSchool(const QString &cName);
    //添加用户到数据库
    //@param cUserName 用户名
    //@param cPasswd 密码
    //@return bool 操作结果
    bool addNewUser(const QString &cUserName,const QString &cPasswd,
                    const QString &cRegisterIP);
    bool adminLogin(const QString &cUserName,
                    const QString &cPassWord);
    bool checkUserOperationJurisdiction(const QString &cUserName);
    //连接数据库
    //@param cHostName 数据库地址
    //@param cPort 数据库端口
    //@param cUserName 数据库用户名
    //@param cPasswd 数据库密码
    //@param cDatabaseName 数据库名称
    //@return bool 连接结果
    bool connectToDatabase(const QString &cHostName,const int &cPort,
                           const QString &cUserName,const QString &cPasswd,
                           const QString &cDatabaseName
                           );
    //删除班级，该操作会将该班级的学生班级信息清空
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cClassName 班级名称
    //@return bool 操作结果
    bool deleteClass(const QString &cSchoolName,const QString &cMajorName,const QString &cClassName);
    //删除专业，该操作会将该专业下的所有班级信息删除，该专业的学生相关信息将被清空
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@return bool 操作结果
    bool deleteMajor(const QString &cSchoolName,const QString &cMajorName);
    //删除学校，该操作会将该学校下的所有专业、班级信息删除，该校的学生相关信息清空
    //@param cSchoolName 学校名称
    //@return bool 操作结果
    bool deleteSchool(const QString &cSchoolName);
    //删除用户
    //@param cUserName 要删除的用户名称
    //@return bool 操作结果
    bool deleteUser(const QString &cUserName);
    //设备校验
    //@param id 设备序号
    //@param cPassWord 密钥
    //@return bool 校验结果
    bool deviceCheck(int id,
                     const QString &cPassWord);
    QJsonArray getAdminList();
    //获取班级ID
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cClassName 班级名称
    //@return qint64 班级ID
    qint64 getClassID(const QString &cSchoolName,
                      const QString &cMajorName,
                      const QString &cClassName);
    //获取某学校某专业下的班级列表
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@return QJsonArray 班级列表
    QJsonArray getClassList(const QString &cSchoolName,const QString &cMajorName);
    //获取当前班级注册状态
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cClassName 班级名称
    //@return bool 是否允许注册
    bool getClassStateOfRegistration(const QString &cSchoolName,
                                     const QString &cMajorName,
                                     const QString &cClassName);
    //获取当前班级注册状态
    //@param id 班级ID
    //@return bool 是否允许注册
    bool getClassStateOfRegistration(int id);
    //获取某学校下的专业列表
    //@param cSchoolName 学校名称
    //@return QStringList 专业列表
    QJsonArray getMajorList(const QString &cSchoolName);
    //获取学校列表
    //@return QStringList 学校列表
    QJsonArray getSchoolList();
    //获取数据库状态
    //@return 当前连接状态
    bool getSqlStat();
    //获取用户信息
    //@return QJsonObject 用户信息
    QJsonObject getUserInfo(const QString &cUserName);
    //获取未提交班级的学生列表
    //@return QStringList 学生列表
    QJsonArray getErrorUserList();
    //获取用户列表
    //@param cSchoolName 学校名称
    //@return QStringList 学生列表
    QJsonArray getUserList(const QString &cSchoolName);
    //获取用户列表
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@return QStringList 学生列表
    QJsonArray getUserList(const QString &cSchoolName,
                            const QString &cMajorName);
    //获取用户列表
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cClassName 班级名称
    //@return QStringList 学生列表
    QJsonArray getUserList(const QString &cSchoolName,
                            const QString &cMajorName,
                            const QString &cClassName);
    //设置班级注册状态
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cClassName 班级名称
    //@param state 是否允许注册
    //@return bool 操作结果
    bool setClassStateOfRegistration(const QString &cSchoolName,
                                     const QString &cMajorName,
                                     const QString &cClassName,
                                     bool state);
    //设置班级注册状态
    //@param id 班级ID
    //@param state 是否允许注册
    //@return bool 操作结果
    bool setClassStateOfRegistration(int id,
                                     bool state);
    //修改管理员密码
    //@param cUserName 用户名
    //@param cOldPasswd 旧密码
    //@param cPasswd 密码
    //@return bool 操作结果
    bool updateAdminPassword(const QString &cUserName, const QString &cPasswd);
    //修改班级名称
    //@param cSchoolName 学校名称
    //@param cMajorName 专业名称
    //@param cOldName 旧名称
    //@param cNewName 新名称
    //@return bool 操作结果
    bool updateClassName(const QString &cSchoolName,
                         const QString &cMajorName,
                         const QString &cOldName,
                         const QString &cNewName);
    //修改专业名称
    //@param cSchoolName 学校名称
    //@param cOldName 旧名称
    //@param cNewName 新名称
    //@return bool 操作结果
    bool updateMajorName(const QString &cSchoolName,
                         const QString &cOldName,
                         const QString &cNewName);
    //修改学校名称
    //@param cOldName 旧名称
    //@param cNewName 新名称
    //@return bool 操作结果
    bool updateSchoolName(const QString &cOldName,
                          const QString &cNewName);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cSchool 学校
    //@param cMajor 专业
    //@param cClass 班级
    //@return bool 操作结果
    bool updateUserClass(const QString &cUserName,
                         const QString &cSchool,
                         const QString &cMajor,
                         const QString &cClass);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cContactInfo 联系方式
    //@return bool 操作结果
    bool updateUserContactInfo(const QString &cUserName,
                               const QString &cContactInfo);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cName 用户姓名
    //@param cSchool 学校
    //@param cMajor 专业
    //@param cClass 班级
    //@param cSchoolNumber 学号
    //@param cPost 职务
    //@param cContactInfo 联系方式
    //@return bool 操作结果
    bool updateUserInfo(const QString &cUserName,
                        const QString &cName,
                        const QString &cSchool,
                        const QString &cMajor,
                        const QString &cClass,
                        const QString &cSchoolNumber,
                        const QString &cPost,
                        const QString &cContactInfo);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cName 用户姓名
    //@param cSchoolNumber 学号
    //@param cPost 职务
    //@param cContactInfo 联系方式
    //@return bool 操作结果
    bool updateUserInfo(const QString &cUserName,
                        const QString &cName,
                        const QString &cSchoolNumber,
                        const QString &cPost,
                        const QString &cContactInfo);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cName 用户姓名
    //@return bool 操作结果
    bool updateUserName(const QString &cUserName,
                        const QString &cName);
    //修改用户密码
    //@param cUserName 用户名
    //@param cOldPasswd 旧密码
    //@param cPasswd 密码
    //@return bool 操作结果
    bool updateUserPassword(const QString &cUserName,const QString &cOldPasswd,const QString &cPasswd);
    bool updateUserPassword(const QString &cUserName,const QString &cPasswd);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cPost 职务
    //@return bool 操作结果
    bool updateUserPost(const QString &cUserName,
                        const QString &cPost);
    //修改用户信息
    //@param cUsername 要修改的用户
    //@param cSchoolNumber 学号
    //@return bool 操作结果
    bool updateUserSchoolNumber(const QString &cUserName,
                                const QString &cSchoolNumber);
    //用户登录
    //@param cUserName 用户名
    //@param cPassWord 密码
    //@return bool 登录结果
    bool userLogin(const QString &cUserName,
                   const QString &cPassWord);    

protected slots:
    void updateTimerTimeoutSlot();

private:
    QSqlDatabase *pMysql = NULL;    //数据库
    QSqlQuery *pQuery = NULL;       //数据库操作对象
    QTimer *pUpdateTimer = NULL;    //定时器
};

#endif // MYSQLDEV_H
