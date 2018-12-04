/*
 * ClassName：Journal
 * Description：用于提供日志存储
 *
 * Author：Albert
 * Version：V0.2
 * Date：2018.09.19
 * Version Description：
 *          【2018.09.19】 V0.2
 *              1.添加日志切割功能
 *              2.增加日志输出功能
 *              3.日志分级
 *          【2017.08.07】 V0.1
 *              1.创建该类。
 *              2.添加日志记录静态函数。
 *              3.讲该类改为单例。
 */

#ifndef JOURNAL_H
#define JOURNAL_H

#include <QObject>
class QFile;
class QTextStream;

class Journal : public QObject
{
    Q_OBJECT
private:
    explicit Journal(QObject *parent = 0);

public:
    static Journal * getobject();
    //[static]记录一条日志
    //@param level 日志级别 数字越小级别越高
    //@param cData 要记录的内容
    static void recordData(quint8 level,const QString &cData);
    //[static]记录一条数字日志
    //@param level 日志级别 数字越小级别越高
    //@param cData 要记录的内容
    static void recordData(quint8 level,qint64 num);
    //[static]记录一条警告日志，日志级别1
    //@param cData 要记录的内容
    static void recordWarnData(const QString &cData);
    //[static]记录一条警告数字日志，日志级别1
    //@param num 要记录的内容
    static void recordWarnData(qint64 num);
    //[static]记录一条错误日志，日志级别0
    //@param cData 要记录的内容
    static void recordErrData(const QString &cData);
    //[static]记录一条错误数字日志，日志级别0
    //@param num 要记录的内容
    static void recordErrData(qint64 num);
    //[static]设置记录级别
    //@param level 级别，会输出0~level之间的日志
    static void setDisplayLevel(quint8 level);
    //[static]设置日志切割的长度，默认为1000
    //@param length 日志切割长度
    static void setCatLen(quint16 length);

protected:

    void proRecordData(const QString &cData);
    void proRecorErrorData(QString type, const QString &cData);
    void proSetCatLen(quint16 length);
    void initLogFile();
    void catLogFile();
    void catErrLogFile();

    QTextStream *fileStream = NULL;
    QFile *file = NULL;
    quint32 fileIndex = 0;
    quint16 jourNum = 0;
    QTextStream *errFileStream = NULL;
    QFile *errFile = NULL;
    quint32 errFileIndex = 0;
    quint16 errJourNum = 0;
    QString oldTime;
    static quint8 recordLevel;
    quint16 catLength = 1000;
};

#endif // JOURNAL_H
