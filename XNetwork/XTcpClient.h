#ifndef XTCPCLIENT_H
#define XTCPCLIENT_H

#include "XSocketBase.h"

class XTcpClientPrivate;
class XTcpClient
{

public:
    XTcpClient();
    virtual ~XTcpClient(){}
    virtual bool Start();
    virtual void Stop();

    virtual int SendData(const char * data, int length);
    virtual int SendDataAsync(const char * data, int length);

private:
    void WorkerProc();

private:
    XTcpClientPrivate *d_ptr;

};

#endif // XTCPCLIENT_H
