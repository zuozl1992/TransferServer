/*
 * ClassName：InfoOutput
 * Description：用于提供运行状态输出函数
 *
 * Author：Albert
 * Version：V0.3
 * Date：2017.07.27
 * Version Description：
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
#include <QTextStream>
#include <QDateTime>
class InfoOutput : public QObject
{
    Q_OBJECT
private:
    explicit InfoOutput(QObject *parent = 0);

public:
    static InfoOutput * getobject();
    static void putOutInfo(const QString &cData);//输出一条普通信息
    static void putOutInfo(const QString &cType,const QJsonObject &cObj);//输出一条JsonObject数据
    static void putOutNum(qint64 num);
    static void putOutErrorInfo(const QString &cErrorData);//输出一条错误信息
    static void putOutErrorNum(qint64 errorNum);//输出一条错误信息

protected:
    void prPutOutInfo(const QString &cData);//输出一条普通信息
    void prPutOutInfo(const QString &cType,const QJsonObject &cObj);//输出一条JsonObject数据
    void prPutOutNum(qint64 num);
    void prPutOutErrorData(const QString &cErrorData);//输出一条错误信息
    void prPutOutErrorNum(qint64 errorNum);
    void prPutOutJson(const QJsonObject &cObj);

private:
    QTextStream *out;//信息输出流
    QTextStream *err;//错误输出流
};

#endif // INFOOUTPUT_H
