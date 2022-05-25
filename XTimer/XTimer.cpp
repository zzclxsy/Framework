#include "XTimer.h"
#include <chrono>
#include "XLogPrint/XLogPrint.h"

XTimer::XTimer():m_iowork(io_ctx)
{
    mb_stop = false;
    m_timer = nullptr;
    m_timer = new boost::asio::steady_timer(io_ctx);
    m_thread = std::thread(std::bind(&XTimer::run,this));
}

XTimer::~XTimer()
{
	if (m_timer)
		 delete m_timer;
}

void XTimer::Start()
{
    m_timer->expires_from_now(boost::asio::chrono::milliseconds(m_interval));
    m_timer->async_wait(std::bind(&XTimer::workPrc,this));
}

void XTimer::Stop()
{
    mb_stop = true;
    m_timer->cancel();
}



void XTimer::workPrc()
{
    if (mb_stop)
    {
        mb_stop = false;
        return;
    }

    m_timerFun();
    if (m_timerType == TimerType::CIRCLE)
    {
        m_timer->expires_at(m_timer->expiry() + boost::asio::chrono::milliseconds(m_interval));
        m_timer->async_wait(std::bind(&XTimer::workPrc,this));
    }
}

void XTimer::run()
{
    io_ctx.run();
}

