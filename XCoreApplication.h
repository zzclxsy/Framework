#ifndef XCOREAPPLICATION_H
#define XCOREAPPLICATION_H
#include <vector>
#include "global.h"
class XEvent;
class XEventLoop;
class XCoreApplicationPrivate;
class FRAMEWORK_EXPORT XCoreApplication
{
public:
    XCoreApplication(int argc, char **argv);
    void exec();
    void quit();
    static XCoreApplication *instant();

private:
    void addEvent(XEvent *event);
    void removeEvent(XEvent *event);
    std::vector<XEvent *> getAllEvent();

private:
    friend XEvent;
    friend XEventLoop;
    XCoreApplicationPrivate *d_ptr;
};

#endif // XCOREAPPLICATION_H
