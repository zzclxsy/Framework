#include "XTcpServer.h"
#include "../Framework/XLogPrint/XLogPrint.h"
#include <boost/bind.hpp>


XTcpServer::XTcpServer()
{
    m_worker = nullptr;
    m_running = false;
}

bool XTcpServer::Start()
{
    if ( m_worker)
    {
        return  m_running;
    }

    m_running = true;
    m_worker = new std::thread([this](){
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
    auto it =  m_sessionMap.begin();
    while (it !=  m_sessionMap.end())
    {
        OnDisconnect((*it));
        it++;
    }
}

void XTcpServer::WorkerProc()
{
    boost::asio::ip::address ipAddress = boost::asio::ip::address::from_string(m_bindIp);
    m_acceptor = new TCP::acceptor(m_ioctx, TCP::endpoint(ipAddress, m_bindPort));
    this->Accept();
    XINFO <<"TcpServer starting success";
    while ( m_running)
    {
        m_ioctx.run_for(std::chrono::milliseconds(100));
    }
}

void XTcpServer::Accept()
{
    XTcpSession * session = new XTcpSession(this);
    m_acceptor->async_accept(
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
        m_sessionMap.insert(session);
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

    m_lock.lock();
    m_sessionMap.erase(session);
    m_trash.push(session);
    if ( m_trash.size() > 20)
    {
        removedSession =  m_trash.front();
        m_trash.pop();
    }
    m_lock.unlock();

    if (removedSession)
    {
        delete removedSession;
    }
}

std::set<XTcpSession *> XTcpServer::totalTcpSession()
{
    return  m_sessionMap;
}

void XTcpServer::SetHeartCheck()
{
    mb_heartCheck = true;
}




XTcpSession::XTcpSession(XTcpServer *tcpServer):m_socket(tcpServer->m_ioctx)
{
    m_tcpServer = tcpServer;
    memset( m_recvBuffer, 0,  TCP_RECV_BUFFER_SIZE);
    m_dataSize = 0;
    m_codec = tcpServer->m_codec;

    m_worker = nullptr;
    m_running = false;
}

XTcpSession::~XTcpSession()
{
    m_running = false;
    if ( m_worker)
    {
        m_worker->join();
        delete  m_worker;
        m_worker = nullptr;
    }
}

boost::asio::ip::tcp::socket &XTcpSession::socket()
{
    return  m_socket;
}

std::string XTcpSession::RemoteIpAddress()
{
    return  m_socket.remote_endpoint().address().to_string();
}

int XTcpSession::RemotePort()
{
    return   m_socket.remote_endpoint().port();
}

int XTcpSession::Start()
{
    if ( m_worker)
    {
        return  m_running;
    }

    m_running = true;
    m_worker = new std::thread([this](){
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
    m_running = false;
    if ( m_worker)
    {
        m_worker->join();
        delete  m_worker;
        m_worker = nullptr;
    }
}

void XTcpSession::WorkerProc()
{
    while ( m_running)
    {
        try
        {
            int dataLen = (int) m_socket.read_some(boost::asio::buffer(& m_recvBuffer[ m_dataSize],  TCP_RECV_BUFFER_SIZE -  m_dataSize));
            m_dataSize += dataLen;

            auto dataDeal = [this](char * data, int length)
            {
                this-> m_tcpServer->OnRecv(this, data, length);
            };

            //是否有解码
            if ( m_codec)
            {
                m_dataSize = m_codec->Decode(
                            m_recvBuffer,
                            m_dataSize,
                            dataDeal);
            }
            else
            {
                dataDeal( m_recvBuffer,  m_dataSize);
                memset( m_recvBuffer, 0,  m_dataSize);
                m_dataSize = 0;
            }
        }
        catch(boost::system::system_error e)
        {
            XERROR << "XTcpSession::WorkerProc, error code:" << e.code();
            m_tcpServer->OnDisconnect(this);
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
            this-> m_socket.send(boost::asio::buffer(p, len));
        }
        catch(boost::system::system_error e)
        {
            XERROR << "XTcpSession::SendData failed, error code:" << e.code();
            this-> m_tcpServer->OnDisconnect(this);
        }
    };

    if ( m_socket.is_open())
    {
        if ( m_codec)
            m_codec->Encode((char *)data, length, sendCallBack);
        else
            sendCallBack((char *)data, length);

        return length;
    }
    return -1;
}

int XTcpSession::RecvDataAsync()
{
    if (! m_socket.is_open())
    {
        return -1;
    }

    m_socket.async_read_some(
                boost::asio::buffer(& m_recvBuffer[ m_dataSize],  TCP_RECV_BUFFER_SIZE -  m_dataSize),
                boost::bind(&XTcpSession::OnRecv,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    return 0;
}

int XTcpSession::SendDataAsync(const char *data, int length)
{
    if (! m_socket.is_open())
    {
        return -1;
    }

    auto sendCallBack = [this](char * p, int len)
    {
        boost::asio::async_write(
                    this-> m_socket,
                    boost::asio::buffer(p, len),
                    boost::bind(&XTcpSession::OnSend,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    };
    if ( m_socket.is_open())
    {
        if ( m_codec)
            m_codec->Encode((char *)data, length, sendCallBack);
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
        m_tcpServer->OnDisconnect(this);
    }
    else if (bytesTransferred > 0)
    {
        m_dataSize += (int)bytesTransferred;

        m_dataSize =  m_codec->Decode(
                    m_recvBuffer,
                    m_dataSize,
                    [this](char * data, int length){
            this-> m_tcpServer->OnRecv(this, data, length);
        });

        this->RecvDataAsync();
    }
}

void XTcpSession::OnSend(const boost::system::error_code &error, size_t bytesTransferred)
{
    if (error)
    {
        XERROR << "XTcpSession::OnSend failed, error code:" << error;
        m_tcpServer->OnDisconnect(this);
    }
    else
    {
        m_tcpServer->OnSend(this, nullptr, (int)bytesTransferred);
    }
}
