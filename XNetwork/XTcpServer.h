#ifndef XTCPSERVER_H
#define XTCPSERVER_H
#include "XSocketBase.h"
#include <set>
#include <queue>

typedef boost::asio::ip::tcp TCP;
class XTcpSessionPrivate;
class XTcpServer;
class XTcpSession
        : public XSocketSession
{
    friend class XTcpServer;

public:
    XTcpSession(XTcpServer * tcpServer);
    ~XTcpSession();

    class Callback
    {
    public:
        virtual void OnRecv(XTcpSession * session, char * data, int length) = 0;
        virtual void OnSend(XTcpSession * session, char * data, int length) = 0;
        virtual void OnDisconnect(XTcpSession * session) = 0;
    };

    TCP::socket &socket();

    virtual std::string RemoteIpAddress();
    virtual int RemotePort();
    virtual int SendData(const char * data, int length);
    virtual int SendDataAsync(const char * data, int length);

protected:
    int Start();
    void Stop();

    void WorkerProc();

    virtual int RecvData(char * buffer, int length);
    virtual int RecvDataAsync();


protected:
    void OnRecv(const boost::system::error_code& error, size_t bytesTransferred);
    void OnSend(const boost::system::error_code& error, size_t bytesTransferred);

protected:
    XTcpSessionPrivate *d_ptr;

};

class XTcpServerPrivate;
class XTcpServer
        : public XSocketServer
        , public XTcpSession::Callback
{
    friend class XTcpSession;
    friend class XTcpSessionPrivate;
    typedef boost::asio::ip::tcp TCP;
public:
    XTcpServer();
    ~XTcpServer(){};

    virtual bool Start();
    virtual void Stop();
    std::set<XTcpSession *> totalTcpSession();
    void SetHeartCheck();

protected:
    void WorkerProc();

    void Accept();
    void OnAccept(XTcpSession * session, const boost::system::error_code& error);

    virtual void OnRecv(XTcpSession * session, char * data, int length);
    virtual void OnSend(XTcpSession * session, char * data, int length);
    virtual void OnDisconnect(XTcpSession * session);

protected:
    XTcpServerPrivate *d_ptr;
};



#endif // XTCPSERVER_H
