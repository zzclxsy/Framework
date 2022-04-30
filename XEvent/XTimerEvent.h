#ifndef XTIMEREVENT_H
#define XTIMEREVENT_H

#include "XEvent.h"
#include <functional>
#include <thread>

class XTimerEvent : public XEvent
{
public:
    XTimerEvent();
    virtual void doWork();
    void setTimer(int msec, std::function<void()> callbakc, bool isSingle = false);
    void setTrigger(int msec);
    void usingThread(bool isUsing);

    void stop();
    void start();

private:
    void runThread();

private:
    int m_interval;
    bool mb_Single;
    std::function<void()> m_callback;
    bool mb_stop;
    unsigned long long m_beforeTime;
    bool mb_thread;
    bool mb_threadFinish;
};

#endif // XTIMEREVENT_H
