#ifndef XTCPCLIENT_H
#define XTCPCLIENT_H

#include "XSocketBase.h"
#include "XTcpHeartPacket.h"
#include "XEvent/XTimerEvent.h"
typedef boost::asio::ip::tcp TCP;
class XTcpClient:public XSocketClient
{

public:
    XTcpClient();
    virtual ~XTcpClient();
    virtual bool Start();
    virtual void Stop();

    virtual int SendData(const char * data, int length);
    virtual int SendDataAsync(const char * data, int length);

    virtual void SetHeartCheck();

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

    std::thread *m_worker;
    bool m_running;
    XTcpHeartPacket m_heartPacket;
    TCP::socket * m_sk;
    TCP::endpoint m_endPoint;
    bool mb_heartCheck;
    XTimerEvent m_connetTimer;

};

#endif // XTCPCLIENT_H
