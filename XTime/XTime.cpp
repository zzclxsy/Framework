#include "XTime.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
XTime* XTime::m_instant = nullptr;
XTime::XTime()
{

}

unsigned long long XTime::getMsecTimestamp()
{
    auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long currTime=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();

    return currTime;
}

unsigned long long XTime::getSecTimestamp()
{
    auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    unsigned long long currTime=std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();

    return currTime;
}

std::string XTime::toTimeString(std::string format)
{
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm t;
#ifdef WIN32
    localtime_s(&t, &now);
#else
    localtime_r(&now, &t);
#endif

    char buff[64] = {0};
    strftime(buff, sizeof (buff), format.c_str(), &t);
    return std::string(buff);
}

void XTime::msleep(int msec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

void XTime::sleep(int sec)
{
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

XTime *XTime::instant()
{
    if (m_instant == nullptr)
        m_instant = new XTime;

    return m_instant;
}
