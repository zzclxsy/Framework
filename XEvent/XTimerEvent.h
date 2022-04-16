#ifndef XTIMEREVENT_H
#define XTIMEREVENT_H

#include "XEvent.h"
#include <functional>
#include <thread>

class XTimerEventPrivate;
class XTimerEvent : public XEvent
{
    typedef struct {

    }S_TimerAttr;
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
    XTimerEventPrivate *d_ptr;
};

#endif // XTIMEREVENT_H
