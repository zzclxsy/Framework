#ifndef XLOGCONSOLEDEVICE_H
#define XLOGCONSOLEDEVICE_H
#include <string>
#include "XLogDeviceBase.h"

class XLogConsoleDevice:public XLogDeviceBase
{
public:
    XLogConsoleDevice();
    virtual ~XLogConsoleDevice();
    virtual bool PrintLog(std::string& log, PriorityLevel level);
};

#endif // XLOGCONSOLEDEVICE_H
