#ifndef VXTCPSERVER_H
#define VXTCPSERVER_H
#include <functional>
#include <string>
#include "VXPacketCodec.h"
#include "VXSocketSession.h"
class VXTcpServer
{
public:
    VXTcpServer(){};
    virtual ~VXTcpServer(){};

    typedef std::function<int (VXSocketSession *, const char *, int)> DataHandler;
    typedef std::function<void(std::string , int)> HeartHandler;

    virtual void SetIpAddress(const std::string& ip) = 0;
    virtual void SetPort(int port) = 0;
    virtual void SetDataHandler(DataHandler handler) = 0;
    virtual void SetPacketCodec(VXPacketCodec * decoder) = 0;
    virtual void SetHeartHander(HeartHandler hander) =0;
    virtual bool Start() = 0;
    virtual void Stop() = 0;
};

#endif // VXTCPSERVER_H
