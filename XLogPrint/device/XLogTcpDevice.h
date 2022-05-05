#ifndef XLOGTCPDEVICE_H
#define XLOGTCPDEVICE_H

#include "XLogDeviceBase.h"
class VXTcpClient;
class XLogTcpDevicePrivate;
class XLogTcpDevice : public XLogDeviceBase
{
public:
    XLogTcpDevice();
    virtual ~XLogTcpDevice(){};
    virtual bool PrintLog(std::string& log, logLevel level);
    virtual bool initialize(const Json::Value &data);

private:
     std::string m_serverIP;
     int m_serverPort;
     std::shared_ptr<VXTcpClient> m_logClient;
};

#endif // XLOGTCPDEVICE_H
