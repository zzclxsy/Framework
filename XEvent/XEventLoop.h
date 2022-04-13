#ifndef XEVENTLOOP_H
#define XEVENTLOOP_H
#include "XEvent.h"
#include "global.h"

class XEventLoopPrivate;
class FRAMEWORK_EXPORT XEventLoop : public XEvent
{
public:
    XEventLoop();
    virtual void doWork(){};
    void exec();
    void quit();

private:
    XEventLoopPrivate *d_ptr;
};

#endif // XEVENTLOOP_H
