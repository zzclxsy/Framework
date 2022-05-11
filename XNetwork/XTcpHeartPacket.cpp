#include "XTcpHeartPacket.h"
#include "XLogPrint/XLogPrint.h"
XTcpHeartPacket::XTcpHeartPacket()
{
    mb_start =false;
    mb_recvHeart = false;

    m_sendHeartTimer = std::make_shared<XTimerEvent>();
    m_timeroutTimer = std::make_shared<XTimerEvent>();

    m_timeroutTimer->setTimer(1000, [&]()
    {
        if (mb_recvHeart)
            return;

        XDEBUG << "no recv heart packet";
        mb_start = false;
        m_sendHeartTimer->stop();
        m_closeSocket();
    }, true);

    m_sendHeartTimer->setTimer(3000, [&]()
    {
        m_sendCallback("heart", 5);
        mb_recvHeart = false;
        m_timeroutTimer->start();
    });
    m_sendHeartTimer->usingThread(true);
}

void XTcpHeartPacket::SetParameter(closeSocket close, sendCallback send)
{
    m_closeSocket = close;
    m_sendCallback = send;
}

bool XTcpHeartPacket::OnRecv(char *data, int length)
{
    if (mb_start == false)
        return false;

    if (length != 5)
        return false;

    if (memcmp(data, "heart", 5) == 0)
    {
        mb_recvHeart = true;
        return true;
    }
    return false;
}

void XTcpHeartPacket::Start()
{
    mb_start = true;
    WorkerProc();
}

void XTcpHeartPacket::Stop()
{
    mb_start = false;
    m_sendHeartTimer->stop();
}

void XTcpHeartPacket::WorkerProc()
{
    XDEBUG <<"start heart.";
    m_sendHeartTimer->start();
}
