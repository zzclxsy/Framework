#include "XEvent.h"
#include "XCoreApplication.h"

class XEventPrivate
{
public:
    XEvent::E_EventType m_type;
    XEvent *q_ptr;
};

XEvent::XEvent()
{
    d_ptr = new XEventPrivate;
    d_ptr->q_ptr = this;
    XCoreApplication::GetApp()->addEvent(this);
}

void XEvent::finishEvent()
{
    XCoreApplication::GetApp()->removeEvent(this);
}

void XEvent::setEventType(E_EventType type)
{
    d_ptr->m_type = type;
}

XEvent::E_EventType XEvent::getEventType()
{
    return d_ptr->m_type;
}
