#include "XTime.h"
#include <chrono>
#include <iostream>
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
    std::string s(30, '\0');
    strftime(&s[0], s.size(), format.c_str(), localtime(&now));
    return s;
}

XTime *XTime::instant()
{
    if (m_instant == nullptr)
        m_instant = new XTime;

    return m_instant;
}
