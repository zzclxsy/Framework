#include "XTimerEvent.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "../XTime/XTime.h"
#include "boost/thread.hpp"

XTimerEvent::XTimerEvent()
{
    mb_stop = true;
    mb_thread = false;
    setEventType(XEvent::E_XTimer);
    mb_threadFinish = true;
}

void XTimerEvent::doWork()
{
    if (mb_stop)
        return;

    unsigned long long currTime= XTime::instant()->getMsecTimestamp();

    //判断线程是否执行完成
    if (mb_threadFinish == false)
        return;

    if (currTime - m_beforeTime >= (unsigned int)m_interval)
    {
        if (mb_thread == false)
        {
            m_callback();
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

void XTimerEvent::setInterval(int msec)
{
    m_interval = msec;
}

void XTimerEvent::setThread(bool isThread)
{
    mb_thread = isThread;
}

void XTimerEvent::stop()
{
    mb_stop = true;
    mb_threadFinish = true;
}

void XTimerEvent::start()
{
    mb_stop = false;
    unsigned long long tmp = XTime::instant()->getMsecTimestamp();
    m_beforeTime = tmp;
}

void XTimerEvent::runThread()
{
    while (mb_threadFinish == false)
    {
        m_callback();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(m_interval));
    }
}
