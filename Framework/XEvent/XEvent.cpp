#include "XEvent.h"
#include "../Framework/XCoreApplication.h"
XEvent::XEvent()
{
    XCoreApplication::instant()->addEvent(this);
}

void XEvent::finishEvent()
{
     XCoreApplication::instant()->removeEvent(this);
}
