#ifndef XTIMEREVENT_H
#define XTIMEREVENT_H

#include "XEvent.h"
#include <functional>

class XTimerEvent : public XEvent
{
    typedef struct {

    }S_TimerAttr;
public:
    XTimerEvent();
    virtual void doWork();
    void setTimer(int ms, std::function<void()> callbakc, bool isSingle = false);
    void stop();
    void start();

private:
    int m_interval;
    bool m_isSingle;
    std::function<void()> m_callback;
    bool m_isStop;
    unsigned long long m_beforeTime;
};

#endif // XTIMEREVENT_H
