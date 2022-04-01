#include "XTimerEvent.h"
#include <chrono>
#include <iostream>
#include <thread>
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

    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long currTime=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();

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
            m_callbackThread = std::thread(std::bind(&XTimerEvent::runThread,this));
            m_callbackThread.detach();
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
}

void XTimerEvent::start()
{
    mb_stop = false;
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    m_beforeTime = tmp;
}

void XTimerEvent::runThread()
{
    m_callback();
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long currTime=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    m_beforeTime = currTime;
    mb_threadFinish = true;
}
