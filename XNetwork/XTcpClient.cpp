#include "XTcpClient.h"
#include <boost/bind.hpp>
#include "../Framework/XLogPrint/XLogPrint.h"




XTcpClient::XTcpClient()
{
     m_worker = nullptr;
     m_running = false;
     m_sk= nullptr;
     mb_heartCheck = false;
     m_linked = E_TCP_LINK::E_Disconnected;
}

XTcpClient::~XTcpClient()
{
    m_running = false;
    if ( m_worker)
    {
         m_worker->join();
        delete  m_worker;
         m_worker = nullptr;
    }

    if ( m_sk)
    {
         m_sk->close();
        delete  m_sk;
         m_sk = nullptr;
    }
}

bool XTcpClient::Start()
{
    if ( m_worker != nullptr)
    {
        return  m_running;
    }

     m_endPoint.address(boost::asio::ip::address_v4::from_string(m_serverIp));
     m_endPoint.port(m_serverPort);
     m_sk = new TCP::socket(m_ioctx);
     m_heartPacket.SetParameter( std::bind(&XTcpClient::closeSocket,this), std::bind(&XTcpClient::SendDataAsync,this,std::placeholders::_1,std::placeholders::_2));

     m_running = true;
     m_worker = new std::thread([this](){
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
     m_running = false;
    if ( m_worker)
    {
         m_worker->join();
        delete  m_worker;
         m_worker = nullptr;
    }

    if ( m_sk)
    {
         m_sk->close();
        delete  m_sk;
         m_sk = nullptr;
    }
}

int XTcpClient::SendData(const char *data, int length)
{
    auto sendCallBack = [this](char * p, int len)
    {
        try
        {
             m_sk->send(boost::asio::buffer(p, len));
        }
        catch(...)
        {
           this->OnDisconnect();
        }
    };

    if ( m_sk->is_open())
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
         m_sk->async_send(
                    boost::asio::buffer(data, length),
                    boost::bind(&XTcpClient::OnSend,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    };

    if ( m_sk->is_open())
    {
        if (m_codec)
            m_codec->Encode((char *)data, length, sendCallBack);
        else
            sendCallBack((char *)data, length);

        return length;
    }
    return -1;
}

void XTcpClient::SetHeartCheck()
{
     mb_heartCheck = true;
}

void XTcpClient::WorkerProc()
{
    this->ConnectAsync();

    while ( m_running)
    {
        if (m_linked == E_Disconnected)
        {
            m_sk->close();
            ConnectAsync();
        }

        m_ioctx.run_for(std::chrono::milliseconds(1000));
    }
}

void XTcpClient::ConnectAsync()
{
    m_linked = E_TCP_LINK::E_Connecting;
    m_sk->async_connect(
                 m_endPoint,
                boost::bind(&XTcpClient::OnConnect,
                            this,
                          boost::asio::placeholders::error));

}

void XTcpClient::OnConnect(const boost::system::error_code &error)
{
    if (error)
    {
        XERROR << "XTcpClient::OnConnect connect error :"<<error;
        ConnectAsync();
    }
    else
    {
        XDEBUG << "XTcpClient::OnConnect\t" <<m_serverIp <<m_serverPort;

        if (m_handler)
        {
            m_handler("connect",7);
            this->RecvDataAsyncCustom();
        }
        else
        {
            this->RecvDataAsync();
        }

        //开始心跳检测
        if ( mb_heartCheck)
             m_heartPacket.Start();

        m_linked = E_TCP_LINK::E_Connected;
    }
}

void XTcpClient::RecvDataAsync()
{
     m_sk->async_read_some(
                boost::asio::buffer(&m_recvBuffer[m_dataSize], RECV_BUFFER_SIZE - m_dataSize),
                boost::bind(&XTcpClient::OnRecv,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void XTcpClient::RecvDataAsyncCustom()
{
     m_sk->async_read_some(
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
            if ( mb_heartCheck ||  m_heartPacket.OnRecv(data, length) == false)
            {
                this->m_dataBuffer.Write(data, length);
                std::unique_lock <std::mutex> lock(this->m_cvLock);
                this->m_cv.notify_one();
            }
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

            if (mb_heartCheck == false ||  m_heartPacket.OnRecv(data, length) == false)
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
    if ( (m_sk->is_open()) &&  m_linked == E_TCP_LINK::E_Connected)
    {
        XERROR << "XTcpClient::OnDisconnect";
        m_sk->close();
        ConnectAsync();
    }
}

void XTcpClient::closeSocket()
{
    if ( (m_sk->is_open()) &&  m_linked == E_TCP_LINK::E_Connected)
    {
        m_linked = E_Disconnected;
    }
}
