#ifndef XEVENT_H
#define XEVENT_H
#include "../global.h"
class XEventPrivate;
class FRAMEWORK_EXPORT XEvent
{
public:
    typedef enum {
        E_XTimer,
        E_XFunction,
        E_XEventLoop,
    }E_EventType;

public:
    XEvent();
    //dowork，做一些事件前置处理
    virtual void doWork() = 0;
    void finishEvent();
    E_EventType getEventType();

protected:
    void setEventType(E_EventType type);

private:
    XEventPrivate *d_ptr;
};

#endif // XEVENT_H
