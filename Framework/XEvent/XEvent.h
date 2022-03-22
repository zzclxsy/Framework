#ifndef XEVENT_H
#define XEVENT_H


class XEvent
{
public:
    typedef enum {
        E_XTimer,
        E_XFunction
    }E_EventType;

public:
    XEvent();
    virtual void doWork() = 0;
    void finishEvent();
};

#endif // XEVENT_H
