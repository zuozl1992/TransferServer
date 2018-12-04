#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QMutex>
#include "journal.h"

#define fout    *fileStream
#define ferr    *errFileStream

quint8 Journal::recordLevel = 1;

Journal::Journal(QObject *parent) : QObject(parent)
{
    initLogFile();
}

void Journal::recordData(quint8 level, const QString &cData)
{
    if(level > recordLevel)
        return;
    Journal::getobject()->proRecordData(cData);
}

void Journal::recordData(quint8 level,qint64 num)
{
    if(level > recordLevel)
        return;
    Journal::getobject()->proRecordData(tr("%1").arg(num));
}

void Journal::recordWarnData(const QString &cData)
{
    if(1 > recordLevel)
        return;
    Journal::getobject()->proRecorErrorData("Warning",cData);
}

void Journal::recordWarnData(qint64 num)
{
    if(1 > recordLevel)
        return;
    Journal::getobject()->proRecorErrorData("Warning",tr("%1").arg(num));
}

void Journal::recordErrData(const QString &cData)
{
    Journal::getobject()->proRecorErrorData("Error",cData);
}

void Journal::recordErrData(qint64 num)
{
    Journal::getobject()->proRecorErrorData("Error",tr("%1").arg(num));
}

void Journal::setDisplayLevel(quint8 level)
{
    recordLevel = level;
}

void Journal::setCatLen(quint16 length)
{
    Journal::getobject()->proSetCatLen(length);
}

Journal *Journal::getobject()
{
    static Journal jou;
    return &jou;
}

void Journal::proRecordData(const QString &cData)
{
    static QMutex jouMutex;
    QMutexLocker locker(&jouMutex);
    if(jourNum>=catLength)
        catLogFile();
    jourNum++;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    fout << str << " " << cData << endl;
}

void Journal::proRecorErrorData(QString type, const QString &cData)
{
    static QMutex jouMutex;
    QMutexLocker locker(&jouMutex);
    if(errJourNum>=catLength)
        catErrLogFile();
    errJourNum++;
    QString str = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz]");
    ferr << str << " " << type << ": " << cData << endl;
}

void Journal::proSetCatLen(quint16 length)
{
    static QMutex jouMutex;
    QMutexLocker locker(&jouMutex);
    catLength = length;
}

void Journal::initLogFile()
{
    oldTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir dir;
    if(!dir.exists("Journal"))
        dir.mkpath("Journal");
    while(QFile(tr("Journal/%1_JournalInfo_%2.log")
                .arg(oldTime)
                .arg(fileIndex)).exists())
        fileIndex++;
    file = new QFile(tr("Journal/%1_JournalInfo_%2.log")
                     .arg(oldTime)
                     .arg(fileIndex));
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    fileStream = new QTextStream(file);
    while(QFile(tr("Journal/%1_JournalError_%2.log")
                .arg(oldTime)
                .arg(errFileIndex)).exists())
        errFileIndex++;
    errFile = new QFile(tr("Journal/%1_JournalError_%2.log")
                        .arg(oldTime)
                        .arg(errFileIndex));
    errFile->open(QIODevice::WriteOnly | QIODevice::Text);
    errFileStream = new QTextStream(errFile);
}

void Journal::catLogFile()
{
    if(oldTime != QDateTime::currentDateTime().toString("yyyy-MM-dd"))
        oldTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    delete fileStream;
    fileStream = NULL;
    file->close();
    delete file;
    file = NULL;
    jourNum = 0;
    while(QFile(tr("Journal/%1_JournalInfo_%2.log")
                .arg(oldTime)
                .arg(fileIndex)).exists())
        fileIndex++;
    file = new QFile(tr("Journal/%1_JournalInfo_%2.log")
                     .arg(oldTime)
                     .arg(fileIndex));
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    fileStream = new QTextStream(file);
}

void Journal::catErrLogFile()
{
    if(oldTime != QDateTime::currentDateTime().toString("yyyy-MM-dd"))
        oldTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    delete errFileStream;
    errFileStream = NULL;
    errFile->close();
    delete errFile;
    errFile = NULL;
    errJourNum = 0;
    while(QFile(tr("Journal/%1_JournalError_%2.log")
                .arg(oldTime)
                .arg(errFileIndex)).exists())
        errFileIndex++;
    errFile = new QFile(tr("Journal/%1_JournalError_%2.log")
                        .arg(oldTime)
                        .arg(errFileIndex));
    errFile->open(QIODevice::WriteOnly | QIODevice::Text);
    errFileStream = new QTextStream(errFile);
}
