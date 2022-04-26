#include "XTimerEvent.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "boost/thread.hpp"
#include "XLogPrint/XLogPrint.h"
#include "XApi/VXApi.h"
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
    d_ptr->mb_threadFinish = true;
    d_ptr->m_beforeTime =  0;
    setEventType(XEvent::E_XTimer);
}

void XTimerEvent::doWork()
{
    if (d_ptr->mb_stop)
        return;

    unsigned long long currTime= XGetTimeModule()->getMsecTimestamp();

    //判断线程是否执行完成
    if (d_ptr->mb_threadFinish == false)
        return;

    if (d_ptr->m_beforeTime == 0)
        return;

    if (currTime - d_ptr->m_beforeTime >= (unsigned long long)d_ptr->m_interval)
    {
        if (d_ptr->mb_thread == false)
        {
            d_ptr->m_callback();
            //单次触发情况
            if (d_ptr->mb_Single)
            {
                d_ptr->mb_stop = true;
                return;;
            }
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

void XTimerEvent::usingThread(bool isUsing)
{
    d_ptr->mb_thread = isUsing;
}

void XTimerEvent::stop()
{
    d_ptr->mb_stop = true;
    d_ptr->mb_threadFinish = true;
}

void XTimerEvent::start()
{
    unsigned long long tmp = XGetTimeModule()->getMsecTimestamp();
    d_ptr->m_beforeTime = tmp;
    d_ptr->mb_stop = false;
}

void XTimerEvent::runThread()
{
    while (d_ptr->mb_threadFinish == false)
    {
        d_ptr->m_callback();
        //单次触发情况
        if (d_ptr->mb_Single)
        {
            d_ptr->mb_threadFinish = true;
            d_ptr->mb_stop = true;
            break;
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(d_ptr->m_interval));
    }
}
