#ifndef XSOCKETBASE_H
#define XSOCKETBASE_H

#include <string>
#include <boost/asio.hpp>
#include <functional>
class JHPacketCodec
{
public:
    JHPacketCodec();
    virtual ~JHPacketCodec();

    typedef std::function<void (char *, int)> Callback;

    virtual int Decode(char * data, int length, Callback callback) = 0;
    virtual int Encode(char * data, int length, Callback callback) = 0;
};


#endif // XSOCKETBASE_H
