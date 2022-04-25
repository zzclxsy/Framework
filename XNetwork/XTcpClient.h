#ifndef XTCPCLIENT_H
#define XTCPCLIENT_H

#include "XSocketBase.h"

class XTcpClientPrivate;
class XTcpClient:public XSocketClient
{

public:
    XTcpClient();
    virtual ~XTcpClient();
    virtual bool Start();
    virtual void Stop();

    virtual int SendData(const char * data, int length);
    virtual int SendDataAsync(const char * data, int length);

private:
    void WorkerProc();
    void ConnectAsync();

    void RecvDataAsync();
    void RecvDataAsyncCustom();

    void OnConnect(const boost::system::error_code& error);
    void OnRecv(const boost::system::error_code& error, size_t bytesTransferred);
    void OnRecvCustom(const boost::system::error_code& error, size_t bytesTransferred);
    void OnSend(const boost::system::error_code& error, size_t bytesTransferred);
    void OnDisconnect();

private:
    XTcpClientPrivate *d_ptr;

};

#endif // XTCPCLIENT_H
