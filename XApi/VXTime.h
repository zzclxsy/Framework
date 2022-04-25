#ifndef VXTIME_H
#define VXTIME_H
#include <string>
class VXTime
{
public:
    virtual unsigned long long getMsecTimestamp() = 0;
    virtual unsigned long long getSecTimestamp() = 0;
    virtual std::string toTimeString(std::string format = "%Y-%m-%d %H:%M:%S") = 0;
    virtual void msleep(int msec) = 0;
    virtual void sleep(int sec) = 0;
};

#endif // VXTIME_H
