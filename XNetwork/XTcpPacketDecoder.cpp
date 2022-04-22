#include "XTcpPacketDecoder.h"

XTcpPacketDecoder::XTcpPacketDecoder()
{

}

int XTcpPacketDecoder::Decode(char *data, int length, Callback callback)
{
    int decodePos = 0;
    while (decodePos + 4 < length)
    {
        Data * p = (Data *)&data[decodePos];
        if (!this->CheckHeader(p))
        {
            return 0;
        }
        int dataLength = this->GetDataLength(p);

        if (decodePos + 4 + dataLength <= length)
        {
            callback(p->data, dataLength);
            decodePos += dataLength + 4;
            continue;
        }
        break;
    }
    if (decodePos > 0 && decodePos < length)
    {
        memmove(data, &data[decodePos], length - decodePos);
    }
    return length - decodePos;
}

int XTcpPacketDecoder::Encode(char *data, int length, Callback callback)
{
    Data d;
    d.header[0] = 0x00;
    d.header[0] = 0x00;
    d.length[0] = (length >> 8) & 0xff;
    d.length[0] = (length) & 0xff;

    int len = 4 + length&0xff;
    char temp[1024];

    memcmp(temp,&d, 4);
    memcmp(&temp[4], data, length);

    callback(temp, len);

    return len;
}

bool XTcpPacketDecoder::CheckHeader(Data *p)
{
    return p->header[0] == 0x00 && p->header[1] == 0x00;
}

int XTcpPacketDecoder::GetDataLength(Data *p)
{
    return ((int)p->length[0] << 8) + (int)p->length[1];
}
