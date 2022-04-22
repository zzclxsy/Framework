#include "XSocketBase.h"


XSocketClient::XSocketClient()
{
    m_codec = nullptr;
    m_recvBuffer = new char[RECV_BUFFER_SIZE];
    m_dataSize = 0;
    m_dataBuffer.SetBufferSize(RECV_BUFFER_SIZE * 10);
}
