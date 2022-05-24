#ifndef XTIMER_H
#define XTIMER_H
#include <boost/function.hpp>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include "thread"
class XTimer
{
public:
    enum TimerType {ONCE, CIRCLE};

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
    friend class TimerManager;

    TimerManager *manager_;
    TimerType timerType_;
    boost::function<void(void)> timerFun_;
    unsigned interval_;
    boost::asio::io_context io_ctx;
    boost::asio::steady_timer *m_timer;
    bool mb_run;
    std::thread *mp_thread;
};

template<typename Fun>
void XTimer::SetParam(Fun fun, unsigned interval, XTimer::TimerType timeType)
{
    interval_ = interval;
    timerFun_ = fun;
    timerType_ = timeType;
}

#endif // XTIMER_H
