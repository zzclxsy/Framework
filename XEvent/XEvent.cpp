#include "XEvent.h"
#include "XCoreApplication.h"



XEvent::XEvent()
{
    XCoreApplication::GetApp()->addEvent(this);
}

void XEvent::finishEvent()
{
    XCoreApplication::GetApp()->removeEvent(this);
}

void XEvent::setEventType(E_EventType type)
{
    m_type = type;
}

XEvent::E_EventType XEvent::getEventType()
{
    return m_type;
}
