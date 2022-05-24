#include "XTimer.h"
#include <chrono>
#include "XLogPrint/XLogPrint.h"

XTimer::XTimer()
{
    m_timer = nullptr;
    mp_thread = nullptr;
}

XTimer::~XTimer()
{
    if (m_timer)
        delete m_timer;

    if (mp_thread)
    {
        mb_run = false;
        mp_thread->join();
        delete mp_thread;
    }
}

void XTimer::Start()
{
    if (m_timer == nullptr)
        m_timer = new boost::asio::steady_timer(io_ctx, boost::asio::chrono::milliseconds(interval_));


    m_timer->async_wait(std::bind(&XTimer::workPrc,this));
    mb_run = true;
    XDEBUG <<"XTimer::Start";
    mp_thread = new std::thread(std::bind(&XTimer::run,this));
}

void XTimer::Stop()
{
    m_timer->cancel();
    mb_run = false;
    if (mp_thread)
    {
        delete mp_thread;
    }
}



void XTimer::workPrc()
{
    XDEBUG <<"XTimer::workPrc";
    timerFun_();
    if (timerType_ == TimerType::CIRCLE)
    {
        m_timer->expires_at(m_timer->expiry() + boost::asio::chrono::milliseconds(interval_));
        m_timer->async_wait(std::bind(&XTimer::workPrc,this));
    }
}

void XTimer::run()
{
    XDEBUG <<"XTimer::run";
    while (mb_run)
    {
        io_ctx.run();
         XDEBUG <<"XTimer::while";
    }
}

