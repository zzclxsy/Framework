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

void XEvent::setEventType(E_EventType type)
{
    m_type = type;
}

XEvent::E_EventType XEvent::getEventType()
{
    return m_type;
}
