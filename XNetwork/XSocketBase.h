#ifndef XSOCKETBASE_H
#define XSOCKETBASE_H

#include <string>
#include "boost/asio.hpp"
#include <functional>
#include "XRingBuffer.h"
#include "XApi/VXPacketCodec.h"
#include "XApi/VXTcpClient.h"
#include "XApi/VXSocketSession.h"
#include "XApi/VXTcpServer.h"
typedef boost::asio::io_context IOContext;

class XSocketClient:public VXTcpClient
{
public:
    XSocketClient();
    virtual ~XSocketClient();

    typedef std::function<int (const char *, int)> DataHandler;

    virtual void SetServerIp(const std::string& ip);
    virtual void SetServerPort(int port);
    virtual void SetDataHandler(DataHandler handler);
    virtual void SetPacketDecoder(VXPacketCodec * decoder);

    virtual XBuffer RecvData(int timeout);

protected:
    IOContext m_ioctx;

    std::string m_serverIp;
    int m_serverPort;
    DataHandler m_handler;
    VXPacketCodec * m_codec;

    std::condition_variable m_cv;
    std::mutex m_cvLock;

    static const int RECV_BUFFER_SIZE = 16384;

    char * m_recvBuffer;
    int m_dataSize;
    XRingBuffer m_dataBuffer;
};

class XSocketServer:public VXTcpServer
{
public:
    XSocketServer();
    virtual ~XSocketServer(){};

    typedef std::function<int (VXSocketSession *, const char *, int)> DataHandler;

    virtual void SetIpAddress(const std::string& ip);
    virtual void SetPort(int port);
    virtual void SetDataHandler(DataHandler handler);
    virtual void SetPacketCodec(VXPacketCodec * decoder);

protected:
    IOContext m_ioctx;

    std::string m_bindIp;
    int m_bindPort;
    DataHandler m_handler;
    VXPacketCodec * m_codec;
};

#endif // XSOCKETBASE_H
