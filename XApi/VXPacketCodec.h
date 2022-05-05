#ifndef VXPACKETCODEC_H
#define VXPACKETCODEC_H
#include <functional>
class VXPacketCodec
{
public:
    VXPacketCodec(){}
    virtual ~VXPacketCodec(){}

    typedef std::function<void (char *, int)> Callback;

    virtual int Decode(char * data, int length, Callback callback) = 0;
    virtual int Encode(char * data, int length, Callback callback) = 0;
};

#endif // VXPACKETCODEC_H
