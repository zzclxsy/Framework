#ifndef VXTCPCLIENT_H
#define VXTCPCLIENT_H
#include <functional>
#include "VXPacketCodec.h"
#include <string>
#include "XRingBuffer.h"
class VXTcpClient
{
public:
    VXTcpClient(){}
    virtual ~VXTcpClient(){}

    typedef std::function<int (const char *, int)> DataHandler;

    virtual void SetServerIp(const std::string& ip) = 0;
    virtual void SetServerPort(int port) = 0;
    virtual void SetDataHandler(DataHandler handler) = 0;
    virtual void SetPacketDecoder(VXPacketCodec * decoder) = 0;

    virtual bool Start() = 0;
    virtual void Stop() = 0;

    virtual int SendData(const char * data, int length) = 0;
    virtual int SendDataAsync(const char * data, int length) = 0;
    virtual XBuffer RecvData(int timeout) = 0;
};

#endif // VXTCPCLIENT_H
