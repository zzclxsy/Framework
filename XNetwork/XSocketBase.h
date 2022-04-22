#ifndef XSOCKETBASE_H
#define XSOCKETBASE_H

#include <string>
#include <boost/asio.hpp>
#include <functional>
#include "../XRingBuffer.h"
#include "../XLogPrint/XLogPrint.h"
typedef boost::asio::io_context IOContext;

class XPacketCodec
{
public:
    XPacketCodec(){}
    virtual ~XPacketCodec(){}

    typedef std::function<void (char *, int)> Callback;

    virtual int Decode(char * data, int length, Callback callback) = 0;
    virtual int Encode(char * data, int length, Callback callback) = 0;
};



class XSocketClient
{
public:
    XSocketClient();
    virtual ~XSocketClient();

    typedef std::function<int (const char *, int)> DataHandler;

    void SetServerIp(const std::string& ip);
    void SetServerPort(int port);
    void SetDataHandler(DataHandler handler);
    void SetPacketDecoder(XPacketCodec * decoder);

    virtual bool Start() = 0;
    virtual void Stop() = 0;

    virtual int SendData(const char * data, int length) = 0;
    virtual int SendDataAsync(const char * data, int length) = 0;
    virtual XBuffer RecvData(int timeout);

protected:
    IOContext m_ioctx;

    std::string m_serverIp;
    int m_serverPort;
    DataHandler m_handler;
    XPacketCodec * m_codec;

    std::condition_variable m_cv;
    std::mutex m_cvLock;

    static const int RECV_BUFFER_SIZE = 16384;

    char * m_recvBuffer;
    int m_dataSize;
    XRingBuffer m_dataBuffer;
};

#endif // XSOCKETBASE_H
