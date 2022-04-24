#ifndef XTCPSERVER_H
#define XTCPSERVER_H
#include "XSocketBase.h"
#include <set>
#include <queue>

class XTcpSession
        : public XSocketSession
{
    friend class XTcpServer;
    typedef boost::asio::ip::tcp TCP;

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

    TCP::socket& socket()
    {
        return m_socket;
    }

    virtual std::string RemoteIpAddress();
    virtual int RemotePort();

protected:
    int Start();
    void Stop();

    void WorkerProc();

    virtual int RecvData(char * buffer, int length);
    virtual int SendData(const char * data, int length);

    virtual int RecvDataAsync();
    virtual int SendDataAsync(const char * data, int length);

protected:
    void OnRecv(const boost::system::error_code& error, size_t bytesTransferred);
    void OnSend(const boost::system::error_code& error, size_t bytesTransferred);

protected:
    TCP::socket m_socket;
    XTcpServer * m_tcpServer;
    XPacketCodec * m_codec;

    std::thread * m_worker;
    bool m_running;

    static const int TCP_RECV_BUFFER_SIZE = 131072;

    char m_recvBuffer[TCP_RECV_BUFFER_SIZE];
    int m_dataSize;
};


class XTcpServer
        : public XSocketServer
        , public XTcpSession::Callback
{
    friend class XTcpSession;
    typedef boost::asio::ip::tcp TCP;
public:
    XTcpServer();
    ~XTcpServer(){};

    virtual bool Start();
    virtual void Stop();

protected:
    void WorkerProc();

    void Accept();
    void OnAccept(XTcpSession * session, const boost::system::error_code& error);

    virtual void OnRecv(XTcpSession * session, char * data, int length);
    virtual void OnSend(XTcpSession * session, char * data, int length);
    virtual void OnDisconnect(XTcpSession * session);

protected:
    std::thread * m_worker;
    bool m_running;

    TCP::acceptor * m_acceptor;
    std::set<XTcpSession *> m_sessionMap;
    std::queue<XTcpSession *> m_trash;
    std::mutex m_lock;
};

#endif // XTCPSERVER_H
