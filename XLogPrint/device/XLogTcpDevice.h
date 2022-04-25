#ifndef XLOGTCPDEVICE_H
#define XLOGTCPDEVICE_H

#include "XLogDeviceBase.h"

class XLogTcpDevicePrivate;
class XLogTcpDevice : public XLogDeviceBase
{
public:
    XLogTcpDevice();
    virtual ~XLogTcpDevice(){};
    virtual bool PrintLog(std::string& log, logLevel level);
};

#endif // XLOGTCPDEVICE_H
