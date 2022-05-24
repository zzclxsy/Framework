#ifndef TIMERWHEEL_H
#define TIMERWHEEL_H


#include <list>
#include <vector>
#include <boost/function.hpp>

namespace TimerWheel{

class TimerManager;
class Timer
{
public:
    enum TimerType {ONCE, CIRCLE};

    Timer();
    Timer(TimerManager* manager);
    ~Timer();

    template<typename Fun>
    void SetParam(Fun fun, unsigned interval, TimerType timeType = CIRCLE);
    void Start();
    void Stop();
    void setManager(TimerManager* manager);

private:
    void OnTimer(unsigned long long now);

private:
    friend class TimerManager;

    TimerManager *manager_;
    TimerType timerType_;
    boost::function<void(void)> timerFun_;
    unsigned interval_;
    unsigned long long expires_;

    int vecIndex_;
    std::list<Timer*>::iterator itr_;
};

class TimerManager
{
public:
    TimerManager();

    static unsigned long long GetCurrentMillisecs();
    void DetectTimers();

    static TimerManager* instant();
    void run();
    void stop();
private:
    friend class Timer;
    void AddTimer(Timer* timer);
    void RemoveTimer(Timer* timer);

    int Cascade(int offset, int index);
    void workPrc();

private:
    typedef std::list<Timer*> TimeList;
    std::vector<TimeList> tvec_;
    unsigned long long checkTime_;
    static TimerManager *mp_instant;
    bool mb_start;
};

template<typename Fun>
inline void Timer::SetParam(Fun fun, unsigned interval, TimerType timeType)
{
    interval_ = interval;
    timerFun_ = fun;
    timerType_ = timeType;
}
}

#endif // TIMERWHEEL_H
