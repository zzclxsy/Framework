#ifndef XTCPHEARTPACKET_H
#define XTCPHEARTPACKET_H
#include <boost/asio.hpp>
#include <functional>
#include "XEvent/XTimerEvent.h"
class XTcpHeartPacket
{
    typedef boost::asio::ip::tcp TCP;
    typedef std::function<int (const char * data, int length)> sendCallback;

public:
    XTcpHeartPacket();
    void SetParameter(TCP::socket *, sendCallback callback);
    bool OnRecv(const char * data, int length);
    void Start();
    void Stop();

private:
    void WorkerProc();

private:
    TCP::socket *mp_socket;
    sendCallback m_callback;
    bool mb_start;
    bool mb_recvHeart;
    XTimerEvent m_sendHeartTimer;
    XTimerEvent m_timeroutTimer;
};

#endif // XTCPHEARTPACKET_H
