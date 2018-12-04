#include "threadhandledev.h"
#ifndef Q_OS_WIN
#include "eventdispatcher_libev.h"
#endif
ThreadHandleDev::ThreadHandleDev()
{
    initfist = false;
}

ThreadHandleDev::~ThreadHandleDev() //停止所有线程，并释放资源
{
    QThread * tmp;
    for (auto it = threadSize.begin(); it != threadSize.end(); ++it)
    {
        tmp = it.key();
        tmp->exit();
        tmp->wait(3000);
        delete tmp;
    }
}

ThreadHandleDev & ThreadHandleDev::getClass()
{
    static ThreadHandleDev thd;
    return thd;
}

QThread *ThreadHandleDev::getThread()
{
    if (!initfist)
    {
        initThreadType(HANDLESIZE,100);
    }
    if (type == THREADSIZE)
        return findThreadSize();
    else
        return findHandleSize();
}

void ThreadHandleDev::removeThread(QThread * thread)
{
    auto t = threadSize.find(thread);
    if (t != threadSize.end())
    {
        t.value() --;
        if (type == HANDLESIZE && t.value() == 0 && threadSize.size() > 1)
        {
            threadSize.remove(thread);
            thread->exit();
            thread->wait(3000);
            delete thread;
        }
    }
}

void ThreadHandleDev::initThreadType(ThreadType type, unsigned int max)
{
    if (!initfist)
    {
        this->type = type;
        this->size = max;
        if (this->size == 0)
        {
            if(type == THREADSIZE)
                this->size = 10;
            else
                this->size = 1000;
        }

        if (type == THREADSIZE)
            initThreadSize();
        else
        {
            QThread * tmp = new QThread;
#ifndef Q_OS_WIN
            tmp->setEventDispatcher(new EventDispatcherLibEv());
#endif
            threadSize.insert(tmp,0);
            tmp->start();
        }
    }
    initfist = true;
}

void ThreadHandleDev::initThreadSize() //建立好线程并启动，
{
    QThread * tmp;
    for (unsigned int i = 0; i < size;++i)
    {
        tmp = new QThread;
#ifndef Q_OS_WIN
        tmp->setEventDispatcher(new EventDispatcherLibEv());
#endif
        threadSize.insert(tmp,0);
        tmp->start();
    }
}

QThread * ThreadHandleDev::findHandleSize() //查找到线程里的连接数小于最大值就返回查找到的，找不到就新建一个线程
{
    for (auto it  = threadSize.begin();it != threadSize.end() ;++it)
    {
        if (it.value() < size)
        {
            it.value() ++;
            return it.key();
        }
    }
    QThread * tmp = new QThread;
#ifndef Q_OS_WIN
    tmp->setEventDispatcher(new EventDispatcherLibEv());
#endif
    threadSize.insert(tmp,1);
    tmp->start();
    return tmp;
}

QThread *ThreadHandleDev::findThreadSize() //遍历查找所有线程中连接数最小的那个，返回
{
    auto it = threadSize.begin();
    auto ite = threadSize.begin();
    for (++it ; it != threadSize.end(); ++it)
    {
        if (it.value() < ite.value())
        {
            ite = it;
        }
    }
    ite.value() ++;
    return ite.key();
}

void ThreadHandleDev::clear()//仅仅清空计数，线程不释放
{
    for (auto it  = threadSize.begin();it != threadSize.end() ;++it)
    {
        it.value()  = 0;
    }
}
