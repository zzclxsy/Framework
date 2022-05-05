#ifndef XEVENTLOOP_H
#define XEVENTLOOP_H
#include "XEvent.h"

class XEventLoop : public XEvent
{
public:
    XEventLoop();
    virtual void doWork(){};
    void exec();
    void quit();

private:
    bool m_isQuit;
};

#endif // XEVENTLOOP_H
