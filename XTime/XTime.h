#ifndef XTIME_H
#define XTIME_H
#include <string>

class XTime
{
public:
    XTime();
    unsigned long long getMsecTimestamp();
    unsigned long long getSecTimestamp();
    std::string toTimeString(std::string format = "%Y-%m-%d %H:%M:%S");

    static XTime *instant();

private:
    static XTime* m_instant;
};

#endif // XTIME_H
