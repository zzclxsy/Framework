#ifndef VXSOCKETSESSION_H
#define VXSOCKETSESSION_H
#include <string>
class VXSocketSession
{
public:
    VXSocketSession(){};
    virtual ~VXSocketSession(){};

    virtual std::string RemoteIpAddress() = 0;
    virtual int RemotePort() = 0;
    virtual int SendData(const char * data, int length) = 0;
    virtual int SendDataAsync(const char * data, int length) = 0;
};

#endif // VXSOCKETSESSION_H
