#include "XTcpClient.h"
#include <boost/bind.hpp>
#include "../Framework/XLogPrint/XLogPrint.h"
typedef boost::asio::ip::tcp TCP;
class XTcpClientPrivate
{
public:
    std::thread *m_worker;
    bool m_running;

    TCP::socket * m_sk;
    TCP::endpoint m_endPoint;
    XTcpClient *q_ptr;
};

XTcpClient::XTcpClient()
{
    d_ptr = new XTcpClientPrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_worker = nullptr;
    d_ptr->m_running = false;
    d_ptr->m_sk= nullptr;
}

bool XTcpClient::Start()
{
    if (d_ptr->m_worker != nullptr)
    {
        return d_ptr->m_running;
    }

    d_ptr->m_endPoint.address(boost::asio::ip::address_v4::from_string(m_serverIp));
    d_ptr->m_endPoint.port(m_serverPort);
    d_ptr->m_sk = new TCP::socket(m_ioctx);

    d_ptr->m_running = true;
    d_ptr->m_worker = new std::thread([this](){
        try{
            this->WorkerProc();
        }
        catch(std::exception e)
        {
            XERROR << "std::exception from XTcpClient.WorkerProc catched:" << e.what();
        }
        catch(...)
        {
            XERROR << "unhandled exception from XTcpClient.WorkerProc catched.";
        }
    });

    return true;
}

void XTcpClient::Stop()
{
    d_ptr->m_running = false;
    if (d_ptr->m_worker)
    {
        d_ptr->m_worker->join();
        delete d_ptr->m_worker;
        d_ptr->m_worker = nullptr;
    }

    if (d_ptr->m_sk)
    {
        d_ptr->m_sk->close();
        delete d_ptr->m_sk;
        d_ptr->m_sk = nullptr;
    }
}

int XTcpClient::SendData(const char *data, int length)
{
    auto sendCallBack = [this](char * p, int len)
    {
        try
        {
            d_ptr->m_sk->send(boost::asio::buffer(p, len));
        }
        catch(...)
        {
           this->OnDisconnect();
        }
    };

    if (d_ptr->m_sk->is_open())
    {
        if (m_codec)
            m_codec->Encode((char *)data, length, sendCallBack);
        else
            sendCallBack((char *)data, length);

        return length;
    }
    return -1;
}

int XTcpClient::SendDataAsync(const char *data, int length)
{
    auto sendCallBack = [this](char * data, int length)
    {
        d_ptr->m_sk->async_send(
                    boost::asio::buffer(data, length),
                    boost::bind(&XTcpClient::OnSend,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    };

    if (d_ptr->m_sk->is_open())
    {
        if (m_codec)
            m_codec->Encode((char *)data, length, sendCallBack);
        else
            sendCallBack((char *)data, length);

        return length;
    }
    return -1;
}

void XTcpClient::WorkerProc()
{
    this->ConnectAsync();

    while (d_ptr->m_running)
    {
        m_ioctx.run_for(std::chrono::milliseconds(100));
    }
}

void XTcpClient::ConnectAsync()
{
    d_ptr->m_sk->async_connect(
                d_ptr->m_endPoint,
                boost::bind(&XTcpClient::OnConnect,
                            this,
                          boost::asio::placeholders::error));
}

void XTcpClient::OnConnect(const boost::system::error_code &error)
{
    if (error)
    {
        XERROR << "XTcpClient::OnConnect failed." <<m_serverIp <<m_serverPort << "error code:" << error;
        this->ConnectAsync();
    }
    else
    {
        XDEBUG << "XTcpClient::OnConnect\t" <<m_serverIp <<m_serverPort;
        m_handler ? this->RecvDataAsyncCustom() : this->RecvDataAsync();
    }
}

void XTcpClient::RecvDataAsync()
{
    d_ptr->m_sk->async_read_some(
                boost::asio::buffer(&m_recvBuffer[m_dataSize], RECV_BUFFER_SIZE - m_dataSize),
                boost::bind(&XTcpClient::OnRecv,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void XTcpClient::RecvDataAsyncCustom()
{
    d_ptr->m_sk->async_read_some(
                boost::asio::buffer(&m_recvBuffer[m_dataSize], RECV_BUFFER_SIZE - m_dataSize),
                boost::bind(&XTcpClient::OnRecvCustom,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void XTcpClient::OnRecv(const boost::system::error_code &error, size_t bytesTransferred)
{
    if (error)
    {
        XERROR << "XTcpClient::OnRecv failed, error code:" << error;
        this->OnDisconnect();
        return;
    }
    else if (bytesTransferred > 0)
    {
        m_dataSize += (int)bytesTransferred;

        auto dataDealCallback = [this](char * data, int length)
        {
            this->m_dataBuffer.Write(data, length);
            std::unique_lock <std::mutex> lock(this->m_cvLock);
            this->m_cv.notify_one();
        };

        if (m_codec)
        {
            m_dataSize =m_codec->Decode(
                        m_recvBuffer,
                        m_dataSize,dataDealCallback);
        }
        else
        {
            dataDealCallback(m_recvBuffer, m_dataSize);
            memset(m_recvBuffer,0,m_dataSize);
            m_dataSize = 0;
        }
    }
    this->RecvDataAsync();
}

void XTcpClient::OnRecvCustom(const boost::system::error_code &error, size_t bytesTransferred)
{
    if (error)
    {
        XERROR << "XTcpClient::OnRecvCustom failed, error code:" << error;
        this->OnDisconnect();
        return;
    }
    else if (bytesTransferred > 0)
    {
        m_dataSize += (int)bytesTransferred;

        auto dataDealCallback = [this](char * data, int length)
        {
            this->m_handler(data, length);
        };

        if (m_codec)
        {
            m_dataSize =m_codec->Decode(
                        m_recvBuffer,
                        m_dataSize,
                        dataDealCallback);
        }
        else
        {
            dataDealCallback(m_recvBuffer, m_dataSize);
            memset(m_recvBuffer,0,m_dataSize);
            m_dataSize = 0;
        }
    }
    this->RecvDataAsyncCustom();
}

void XTcpClient::OnSend(const boost::system::error_code &error, size_t bytesTransferred)
{
    if (error)
    {
        XERROR << "XTcpClient::OnSend failed, error code:" << error;
        this->OnDisconnect();
    }
    else if (bytesTransferred <= 0)
    {
        XERROR << "XTcpClient::OnSend, error bytes transferred";
    }
}

void XTcpClient::OnDisconnect()
{
    XERROR << "XTcpClient::OnDisconnect ";
    d_ptr->m_sk->close();
    this->ConnectAsync();
}
