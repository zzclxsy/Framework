#ifndef XTIMEREVENT_H
#define XTIMEREVENT_H

#include "XEvent.h"
#include <functional>
#include <thread>

class XTimerEvent : public XEvent
{
    typedef struct {

    }S_TimerAttr;
public:
    XTimerEvent();
    virtual void doWork();
    void setTimer(int msec, std::function<void()> callbakc, bool isSingle = false);
    void setInterval(int msec);
    void setThread(bool isThread);

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
    std::thread m_callbackThread;
};

#endif // XTIMEREVENT_H
