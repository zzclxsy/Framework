#include "XTimerEvent.h"
#include <chrono>
#include <iostream>
XTimerEvent::XTimerEvent()
{
    m_isStop = true;
}

void XTimerEvent::doWork()
{
    if (m_isStop)
        return;

    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long currTime=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();

    if (currTime - m_beforeTime >= (unsigned int)m_interval)
    {
        m_callback();
        m_beforeTime = currTime;
    }
}

void XTimerEvent::setTimer(int ms, std::function<void ()> callbakc, bool isSingle)
{
    m_interval = ms;
    m_isSingle = isSingle;
    m_callback = callbakc;
}

void XTimerEvent::stop()
{
    m_isStop = true;
}

void XTimerEvent::start()
{
    m_isStop = false;
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    m_beforeTime = tmp;
}
