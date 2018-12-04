/*
 * ClassName：InfoOutput
 * Description：状态输出
 *
 * Author：Albert
 * Version：V1.0
 * Date：2018.09.18
 * Version Description：
 *          【2018.09.18】 V1.0
 *              1.优化结构
 *              2.修改接口名称
 *          【2017.07.27】 V0.3
 *              1.修改错误输出信息输出方式，统一格式。
 *              2.为所有信息输出添加毫秒显示。
 *          【2017.07.26】 V0.2
 *              1.修改错误信息输出格式。
 *          【2017.07.19】 V0.1
 *              1.创建该类
 *              2.将类修改为单例类型。
 *              3.添加信息输出函数。
 */

#ifndef INFOOUTPUT_H
#define INFOOUTPUT_H

#include <QObject>

class QTextStream;

class InfoOutput : public QObject
{
    Q_OBJECT
private:
    explicit InfoOutput(QObject *parent = 0);

public:
    static InfoOutput * getobject();
    //[static]输出一条状态信息
    //@param cData 要输出的信息
    static void putOutInfo(const QString &cData);
    //[static]输出一个数字
    //@param num 要输出的数字
    static void putOutInfo(qint64 num);
    //[static]输出一条Bool状态信息
    //@param cType 要输出的类型
    //@param re 要输出的结果
    static void putOutInfo(const QString &cType,bool re);
    //[static]输出一条JsonObject信息
    //@param cType 要输出的类型
    //@param cObj 要输出的JSON数据
    static void putOutInfo(const QString &cType,const QJsonObject &cObj);
    //[static]输出一条错误信息
    //@param 要输出的信息
    static void putOutErrorInfo(const QString &cErrorData);
    //[static]输出一个出错的数字
    //@param num 要输出的数字
    static void putOutErrorInfo(qint64 num);
    //[static]输出一条出错的Bool状态信息
    //@param cType 要输出的类型
    //@param re 要输出的结果
    static void putOutErrorInfo(const QString &cData,bool re);
    //[static]输出一条错误信息
    //@param 要输出的信息
    static void putOutWarningInfo(const QString &cErrorData);
    //[static]输出一个出错的数字
    //@param num 要输出的数字
    static void putOutWarningInfo(qint64 num);
    //[static]输出一条出错的Bool状态信息
    //@param cType 要输出的类型
    //@param re 要输出的结果
    static void putOutWarningInfo(const QString &cData,bool re);
    //[static]设置输出开关
    //@param sw 是否要显示输出
    static void setDisplaySwitch(bool sw);

protected:

    void proPutOutInfo(const QString &cData);
    void proPutOutInfo(const QString &cType,const QJsonObject &cObj);
    void proPutOutInfo(const QString &cData,bool re);
    void proPutOutErrorInfo(QString type,const QString &cErrorData);
    void proPutOutErrorInfo(QString type,const QString &cData,bool re);
    void proPutOutJson(const QJsonObject &value);
    void proSetDisplaySwitch(bool sw);

    QTextStream *out;
    QTextStream *err;
    bool displaySwitch = true;
};

#endif // INFOOUTPUT_H
