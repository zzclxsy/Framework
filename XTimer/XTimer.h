#ifndef XTIMER_H
#define XTIMER_H
#include <boost/function.hpp>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include "thread"
class XTimer
{
public:
    enum TimerType {ONCE = 0, CIRCLE = 1};

    XTimer();
    ~XTimer();

    template<typename Fun>
    void SetParam(Fun fun, unsigned interval, TimerType timeType = CIRCLE);
    void Start();
    void Stop();

private:
    void workPrc();
    void run();

private:
    TimerType m_timerType;
    boost::function<void(void)> m_timerFun;
    unsigned m_interval;
    boost::asio::io_context io_ctx;
    boost::asio::steady_timer *m_timer;
    std::thread m_thread;
    bool mb_stop;
    boost::asio::io_service::work m_iowork;
};

template<typename Fun>
void XTimer::SetParam(Fun fun, unsigned interval, XTimer::TimerType timeType)
{
    m_interval = interval;
    m_timerFun = fun;
    m_timerType = timeType;
}

#endif // XTIMER_H
