#include "XTimerEvent.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "boost/thread.hpp"
#include "XLogPrint/XLogPrint.h"
#include "XApi/VXApi.h"


XTimerEvent::XTimerEvent()
{
    mb_stop = true;
    mb_thread = false;
    mb_threadFinish = true;
    m_beforeTime =  0;
    setEventType(XEvent::E_XTimer);
}

void XTimerEvent::doWork()
{
    if (mb_stop)
        return;

    unsigned long long currTime= XGetTimeModule()->getMsecTimestamp();

    //判断线程是否执行完成
    if (mb_threadFinish == false)
        return;

    if (m_beforeTime == 0)
        return;

    if (currTime - m_beforeTime >= (unsigned long long)m_interval)
    {
        if (mb_thread == false)
        {
            m_callback();
            //单次触发情况
            if (mb_Single)
            {
                mb_stop = true;
                return;;
            }
            m_beforeTime = currTime;
        }
        else
        {
            mb_threadFinish = false;
            boost::thread thread(std::bind(&XTimerEvent::runThread,this));
        }
    }
}

void XTimerEvent::setTimer(int msec, std::function<void ()> callbakc, bool isSingle)
{
    m_interval = msec;
    mb_Single = isSingle;
    m_callback = callbakc;
}

void XTimerEvent::setTrigger(int msec)
{
    m_interval = msec;
}

void XTimerEvent::usingThread(bool isUsing)
{
    mb_thread = isUsing;
}

void XTimerEvent::stop()
{
   mb_stop = true;
    mb_threadFinish = true;
}

void XTimerEvent::start()
{
    unsigned long long tmp = XGetTimeModule()->getMsecTimestamp();
    m_beforeTime = tmp;
    mb_stop = false;
}

void XTimerEvent::runThread()
{
    while (mb_threadFinish == false)
    {
        m_callback();
        //单次触发情况
        if (mb_Single)
        {
            mb_threadFinish = true;
            mb_stop = true;
            break;
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(m_interval));
    }
}
