#include "XTcpHeartPacket.h"
#include "XLogPrint/XLogPrint.h"
XTcpHeartPacket::XTcpHeartPacket()
{
    mb_start =false;
    mp_socket = nullptr;
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
			mp_socket->close();
		}, true);

	m_sendHeartTimer->setTimer(3000, [&]()
		{
			m_callback("heart", 5);
			mb_recvHeart = false;
			m_timeroutTimer->start();
		});
	m_sendHeartTimer->usingThread(true);
}

void XTcpHeartPacket::SetParameter(boost::asio::ip::tcp::socket * sock, sendCallback callback)
{
    mp_socket = sock;
    m_callback = callback;
}

bool XTcpHeartPacket::OnRecv(const char *data, int length)
{
    if (mb_start == false)
        return false;

    if (length != 5)
        return false;

    if (mp_socket->is_open() == false)
    {
        Stop();
        return false;
    }

    char buf[5];
    memcpy(buf, data, length);

    std::string str(buf);

    if (str == "heart")
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
