#include "XTimerEvent.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "../XTime/XTime.h"
#include "boost/thread.hpp"

class XTimerEventPrivate
{
public:
    int m_interval;
    bool mb_Single;
    std::function<void()> m_callback;
    bool mb_stop;
    unsigned long long m_beforeTime;
    bool mb_thread;
    bool mb_threadFinish;

    XTimerEvent *q_ptr;
};

XTimerEvent::XTimerEvent()
{
    d_ptr = new XTimerEventPrivate;
    d_ptr->q_ptr = this;
    d_ptr->mb_stop = true;
    d_ptr->mb_thread = false;
    setEventType(XEvent::E_XTimer);
    d_ptr->mb_threadFinish = true;
}

void XTimerEvent::doWork()
{
    if (d_ptr->mb_stop)
        return;

    unsigned long long currTime= XTime::instant()->getMsecTimestamp();

    //判断线程是否执行完成
    if (d_ptr->mb_threadFinish == false)
        return;

    if (currTime - d_ptr->m_beforeTime >= (unsigned int)d_ptr->m_interval)
    {
        if (d_ptr->mb_thread == false)
        {
            d_ptr->m_callback();
            d_ptr->m_beforeTime = currTime;
        }
        else
        {
            d_ptr->mb_threadFinish = false;
            boost::thread thread(std::bind(&XTimerEvent::runThread,this));
        }
    }
}

void XTimerEvent::setTimer(int msec, std::function<void ()> callbakc, bool isSingle)
{
    d_ptr->m_interval = msec;
    d_ptr->mb_Single = isSingle;
    d_ptr->m_callback = callbakc;
}

void XTimerEvent::setTrigger(int msec)
{
    d_ptr->m_interval = msec;
}

void XTimerEvent::setThreadModel(bool isThread)
{
    d_ptr->mb_thread = isThread;
}

void XTimerEvent::stop()
{
    d_ptr->mb_stop = true;
    d_ptr->mb_threadFinish = true;
}

void XTimerEvent::start()
{
    d_ptr->mb_stop = false;
    unsigned long long tmp = XTime::instant()->getMsecTimestamp();
    d_ptr->m_beforeTime = tmp;
}

void XTimerEvent::runThread()
{
    while (d_ptr->mb_threadFinish == false)
    {
        d_ptr->m_callback();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(d_ptr->m_interval));
    }
}
