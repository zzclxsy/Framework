#ifndef XTIME_H
#define XTIME_H
#include <string>
#include "XApi/VXModule.h"
#include "XApi/VXTime.h"
#include "XApi/VXFactory.h"
class  XTime:
        public VXModule,
        public VXTime
{
public:
    XTime();
    virtual ~XTime(){}

    virtual unsigned long long getMsecTimestamp();
    virtual unsigned long long getSecTimestamp();
    virtual std::string toTimeString(std::string format = "%Y-%m-%d %H:%M:%S");
    virtual void msleep(int msec);
    virtual void sleep(int sec);
};

X_FACTORY_DECL(XTime)

#endif // XTIME_H
