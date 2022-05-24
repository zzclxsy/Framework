#ifndef XTCPSERVER_H
#define XTCPSERVER_H
#include "XSocketBase.h"
#include <set>
#include <queue>
#include "XApi/VXPacketCodec.h"
#include "XTcpHeartPacket.h"
typedef boost::asio::ip::tcp TCP;
class XTcpSession
        : public VXSocketSession
{
    friend class XTcpServer;

public:
    XTcpSession(XTcpServer * tcpServer);
    ~XTcpSession();

    enum LinkState{
        linking,
        linked,
        disLink
    };

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

    void closeSocket();

protected:
    int Start();
    void Stop();

    void WorkerProc();

    int RecvData(char * buffer, int length);
    int RecvDataAsync();


protected:
    void OnRecv(const boost::system::error_code& error, size_t bytesTransferred);
    void OnSend(const boost::system::error_code& error, size_t bytesTransferred);

protected:
    TCP::socket m_socket;
    XTcpServer * m_tcpServer;
    VXPacketCodec * m_codec;
    LinkState m_link;
    std::thread * m_worker;
    bool m_running;
    XTcpHeartPacket m_heartPacket;
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
    std::set<XTcpSession *> totalTcpSession();
    void SetHeartCheck();

protected:
    void WorkerProc();

    void Accept();
    void OnAccept(XTcpSession * session, const boost::system::error_code& error);

    virtual void OnRecv(XTcpSession * session, char * data, int length);
    virtual void OnSend(XTcpSession * session, char * data, int length);
    virtual void OnDisconnect(XTcpSession * session);
    virtual void SetHeartHander(HeartHandler hander);
protected:
    std::thread * m_worker;
    bool m_running;
    bool mb_heartCheck;
    TCP::acceptor * m_acceptor;
    std::set<XTcpSession *> m_sessionMap;
    std::queue<XTcpSession *> m_trash;
    std::mutex m_lock;
    HeartHandler m_handle;
};



#endif // XTCPSERVER_H
