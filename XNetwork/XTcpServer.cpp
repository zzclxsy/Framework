#include "XTcpServer.h"
#include "../Framework/XLogPrint/XLogPrint.h"
#include <boost/bind.hpp>

class XTcpServerPrivate
{
public:
    std::thread * m_worker;
    bool m_running;

    TCP::acceptor * m_acceptor;
    std::set<XTcpSession *> m_sessionMap;
    std::queue<XTcpSession *> m_trash;
    std::mutex m_lock;
};

XTcpServer::XTcpServer()
{
    d_ptr = new XTcpServerPrivate;
    d_ptr->m_worker = nullptr;
    d_ptr->m_running = false;
}

bool XTcpServer::Start()
{
    if (d_ptr->m_worker)
    {
        return d_ptr->m_running;
    }

    d_ptr->m_running = true;
    d_ptr->m_worker = new std::thread([this](){
        try{
            this->WorkerProc();
        }
        catch(std::exception e)
        {
            XERROR << "std::exception from JHTcpServer.WorkerProc catched:" << e.what();
        }
        catch(...)
        {
            XERROR << "unhandled exception from JHTcpServer.WorkerProc catched.";
        }
    });

    return true;
}

void XTcpServer::Stop()
{

}

void XTcpServer::WorkerProc()
{
    boost::asio::ip::address ipAddress = boost::asio::ip::address::from_string(m_bindIp);
    d_ptr->m_acceptor = new TCP::acceptor(m_ioctx, TCP::endpoint(ipAddress, m_bindPort));
    this->Accept();
    XINFO <<"TcpServer starting success";
    while (d_ptr->m_running)
    {
        m_ioctx.run_for(std::chrono::milliseconds(100));
    }
}

void XTcpServer::Accept()
{
    XTcpSession * session = new XTcpSession(this);
    d_ptr->m_acceptor->async_accept(
                session->socket(),
                boost::bind(
                    &XTcpServer::OnAccept,
                    this,
                    session,
                    boost::asio::placeholders::error));
}

void XTcpServer::OnAccept(XTcpSession *session, const boost::system::error_code &error)
{
    if (!error)
    {
        d_ptr->m_sessionMap.insert(session);
        session->Start();
        XINFO << session->RemoteIpAddress() <<"connect succeed" << " port :" << session->RemotePort();
        this->Accept();
    }
    else
    {
        XDEBUG << "XTcpServer::OnAccept, error code:" << error;
        this->OnDisconnect(session);
    }
}

void XTcpServer::OnRecv(XTcpSession *session, char *data, int length)
{
    if (m_handler)
    {
        m_handler(session, data, length);
    }
}

void XTcpServer::OnSend(XTcpSession *session, char *data, int length)
{

}

void XTcpServer::OnDisconnect(XTcpSession *session)
{
    XTcpSession * removedSession = nullptr;

    d_ptr->m_lock.lock();
    d_ptr->m_sessionMap.erase(session);
    d_ptr->m_trash.push(session);
    if (d_ptr->m_trash.size() > 20)
    {
        removedSession = d_ptr->m_trash.front();
        d_ptr->m_trash.pop();
    }
    d_ptr->m_lock.unlock();

    if (removedSession)
    {
        delete removedSession;
    }
}

std::set<XTcpSession *> XTcpServer::totalTcpSession()
{
    return d_ptr->m_sessionMap;
}




class XTcpSessionPrivate
{
public:
    XTcpSessionPrivate(XTcpServer *tcpServer):m_socket(tcpServer->m_ioctx){}

    TCP::socket m_socket;
    XTcpServer * m_tcpServer;
    VXPacketCodec * m_codec;

    std::thread * m_worker;
    bool m_running;

    static const int TCP_RECV_BUFFER_SIZE = 131072;

    char m_recvBuffer[TCP_RECV_BUFFER_SIZE];
    int m_dataSize;
};

XTcpSession::XTcpSession(XTcpServer *tcpServer)
{
    d_ptr = new XTcpSessionPrivate(tcpServer);

    d_ptr->m_tcpServer = tcpServer;
    memset(d_ptr->m_recvBuffer, 0, d_ptr->TCP_RECV_BUFFER_SIZE);
    d_ptr->m_dataSize = 0;
    d_ptr->m_codec = tcpServer->m_codec;

    d_ptr->m_worker = nullptr;
    d_ptr->m_running = false;
}

XTcpSession::~XTcpSession()
{
    d_ptr->m_running = false;
    if (d_ptr->m_worker)
    {
        d_ptr->m_worker->join();
        delete d_ptr->m_worker;
        d_ptr->m_worker = nullptr;
    }
}

boost::asio::ip::tcp::socket &XTcpSession::socket()
{
    return d_ptr->m_socket;
}

std::string XTcpSession::RemoteIpAddress()
{
    return d_ptr->m_socket.remote_endpoint().address().to_string();
}

int XTcpSession::RemotePort()
{
    return  d_ptr->m_socket.remote_endpoint().port();
}

int XTcpSession::Start()
{
    if (d_ptr->m_worker)
    {
        return d_ptr->m_running;
    }

    d_ptr->m_running = true;
    d_ptr->m_worker = new std::thread([this](){
        try{
            this->WorkerProc();
        }
        catch(std::exception e)
        {
            XERROR << "std::exception from XTcpSession.WorkerProc catched:" << e.what();
        }
        catch(...)
        {
            XERROR << "unhandled exception from XTcpSession.WorkerProc catched.";
        }
    });

    return 0;
}

void XTcpSession::Stop()
{
    d_ptr->m_running = false;
    if (d_ptr->m_worker)
    {
        d_ptr->m_worker->join();
        delete d_ptr->m_worker;
        d_ptr->m_worker = nullptr;
    }
}

void XTcpSession::WorkerProc()
{
    while (d_ptr->m_running)
    {
        try
        {
            int dataLen = (int)d_ptr->m_socket.read_some(boost::asio::buffer(&d_ptr->m_recvBuffer[d_ptr->m_dataSize], d_ptr->TCP_RECV_BUFFER_SIZE - d_ptr->m_dataSize));
            d_ptr->m_dataSize += dataLen;

            auto dataDealCallback = [this](char * data, int length)
            {
                this->d_ptr->m_tcpServer->OnRecv(this, data, length);
            };

            //是否有解码
            if (d_ptr->m_codec)
            {
                d_ptr->m_dataSize =d_ptr->m_codec->Decode(
                            d_ptr->m_recvBuffer,
                            d_ptr->m_dataSize,
                            dataDealCallback);
            }
            else
            {
                dataDealCallback(d_ptr->m_recvBuffer, d_ptr->m_dataSize);
                memset(d_ptr->m_recvBuffer, 0, d_ptr->m_dataSize);
                d_ptr->m_dataSize = 0;
            }
        }
        catch(boost::system::system_error e)
        {
            XERROR << "XTcpSession::WorkerProc, error code:" << e.code();
            d_ptr->m_tcpServer->OnDisconnect(this);
            break;
        }
    }
}

int XTcpSession::RecvData(char *buffer, int length)
{
    (void)buffer;
    (void)length;

    return -1;
}

int XTcpSession::SendData(const char *data, int length)
{
    auto sendCallBack = [this](char * p, int len)
    {
        try
        {
            this->d_ptr->m_socket.send(boost::asio::buffer(p, len));
        }
        catch(boost::system::system_error e)
        {
            XERROR << "XTcpSession::SendData failed, error code:" << e.code();
            this->d_ptr->m_tcpServer->OnDisconnect(this);
        }
    };

    if (d_ptr->m_socket.is_open())
    {
        if (d_ptr->m_codec)
            d_ptr->m_codec->Encode((char *)data, length, sendCallBack);
        else
            sendCallBack((char *)data, length);

        return length;
    }
    return -1;
}

int XTcpSession::RecvDataAsync()
{
    if (!d_ptr->m_socket.is_open())
    {
        return -1;
    }

    d_ptr->m_socket.async_read_some(
                boost::asio::buffer(&d_ptr->m_recvBuffer[d_ptr->m_dataSize], d_ptr->TCP_RECV_BUFFER_SIZE - d_ptr->m_dataSize),
                boost::bind(&XTcpSession::OnRecv,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    return 0;
}

int XTcpSession::SendDataAsync(const char *data, int length)
{
    if (!d_ptr->m_socket.is_open())
    {
        return -1;
    }

    auto sendCallBack = [this](char * p, int len)
    {
        boost::asio::async_write(
                    this->d_ptr->m_socket,
                    boost::asio::buffer(p, len),
                    boost::bind(&XTcpSession::OnSend,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    };
    if (d_ptr->m_socket.is_open())
    {
        if (d_ptr->m_codec)
            d_ptr->m_codec->Encode((char *)data, length, sendCallBack);
        else
            sendCallBack((char *)data, length);
    }
    return 0;
}

void XTcpSession::OnRecv(const boost::system::error_code &error, size_t bytesTransferred)
{
    if (error)
    {
        XERROR << "XTcpSession::OnRecv failed, error code:" << error;
        d_ptr->m_tcpServer->OnDisconnect(this);
    }
    else if (bytesTransferred > 0)
    {
        d_ptr->m_dataSize += (int)bytesTransferred;

        d_ptr->m_dataSize = d_ptr->m_codec->Decode(
                    d_ptr->m_recvBuffer,
                    d_ptr->m_dataSize,
                    [this](char * data, int length){
            this->d_ptr->m_tcpServer->OnRecv(this, data, length);
        });

        this->RecvDataAsync();
    }
}

void XTcpSession::OnSend(const boost::system::error_code &error, size_t bytesTransferred)
{
    if (error)
    {
        XERROR << "XTcpSession::OnSend failed, error code:" << error;
        d_ptr->m_tcpServer->OnDisconnect(this);
    }
    else
    {
        d_ptr->m_tcpServer->OnSend(this, nullptr, (int)bytesTransferred);
    }
}
