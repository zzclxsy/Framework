#ifndef XTCPPACKETDECODER_H
#define XTCPPACKETDECODER_H
#include "XSocketBase.h"

class XTcpPacketDecoder:public XPacketCodec
{
public:
    XTcpPacketDecoder();

    struct Data
    {
        unsigned char header[2];
        unsigned char length[2];
        char data[0];
    };

    virtual int Decode(char * data, int length, Callback callback);
    virtual int Encode(char * data, int length, Callback callback);

protected:
    bool CheckHeader(Data * p);
    int GetDataLength(Data * p);
};

#endif // XTCPPACKETDECODER_H
