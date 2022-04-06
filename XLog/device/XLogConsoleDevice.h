#ifndef XLOGCONSOLEDEVICE_H
#define XLOGCONSOLEDEVICE_H
#include <string>
#include "XLogDeviceBase.h"

class XLogConsoleDevice:public XLogDeviceBase
{
public:
    XLogConsoleDevice();
    bool PrintLog(std::string log, XLogRule::PriorityLevel level);

    void debugPrint(std::string log);
};

#endif // XLOGCONSOLEDEVICE_H
