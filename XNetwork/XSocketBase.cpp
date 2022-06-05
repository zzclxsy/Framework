#include "XSocketBase.h"
#include "XLogPrint/XLogPrint.h"

XSocketClient::XSocketClient()
{
	m_codec = nullptr;
	m_recvBuffer = new char[RECV_BUFFER_SIZE];
	m_dataSize = 0;
	m_dataBuffer.SetBufferSize(RECV_BUFFER_SIZE * 10);
}

XSocketClient::~XSocketClient()
{
    delete m_recvBuffer;
    delete m_codec;
}

void XSocketClient::SetServerIp(const std::string &ip)
{
    m_serverIp = ip;
}

void XSocketClient::SetServerPort(int port)
{
    m_serverPort = port;
}

void XSocketClient::SetDataHandler(DataHandler handler)
{
    m_handler = handler;
}

void XSocketClient::SetPacketDecoder(VXPacketCodec *decoder)
{
    m_codec = decoder;
}

XBuffer XSocketClient::RecvData(int timeout)
{
    if (!m_dataBuffer.IsEmpty()
            || !timeout)
    {
        return m_dataBuffer.Read();
    }

    std::unique_lock <std::mutex> lock(m_cvLock);
    m_cv.wait_for(lock, std::chrono::milliseconds(timeout));

    return m_dataBuffer.Read();
}

XSocketServer::XSocketServer()
{
    m_bindIp = "0.0.0.0";
    m_codec = nullptr;
}

void XSocketServer::SetIpAddress(const std::string &ip)
{
    m_bindIp = ip;
}

void XSocketServer::SetPort(int port)
{
    m_bindPort = port;
}

void XSocketServer::SetDataHandler(DataHandler handler)
{
    m_handler = handler;
}

void XSocketServer::SetPacketCodec(VXPacketCodec *decoder)
{
    m_codec = decoder;
}

void XSocketServer::SetHeartHander(HeartHandler hander)
{
    m_heartHandler = hander;
}
