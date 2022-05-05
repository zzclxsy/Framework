#ifndef XTCPHEARTPACKET_H
#define XTCPHEARTPACKET_H
#include <functional>
#include "XEvent/XTimerEvent.h"
#include <memory>
class XTcpHeartPacket
{
    typedef std::function<int (const char * data, int length)> sendCallback;
    typedef std::function<void ()> closeSocket;
public:
    XTcpHeartPacket();
    void SetParameter(closeSocket close, sendCallback send);
    bool OnRecv(char * data, int length);
    void Start();
    void Stop();

private:
    void WorkerProc();

private:
    sendCallback m_sendCallback;
    closeSocket m_closeSocket;
    bool mb_start;
    bool mb_recvHeart;
    std::shared_ptr<XTimerEvent> m_sendHeartTimer;
    std::shared_ptr<XTimerEvent> m_timeroutTimer;
};

#endif // XTCPHEARTPACKET_H
