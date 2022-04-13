#include "XEventLoop.h"
#include "XCoreApplication.h"
#include <iostream>

class XEventLoopPrivate
{
public:
    bool m_isQuit;
    XEventLoop *q_ptr;
};

XEventLoop::XEventLoop()
{
    d_ptr = new XEventLoopPrivate;
    d_ptr->q_ptr = this;
    d_ptr->m_isQuit = false;
    setEventType(XEvent::E_XEventLoop);
}


void XEventLoop::exec()
{
    std::vector<XEvent *> m_allEvent = XCoreApplication::instant()->getAllEvent();

    int index = 0;

    while(!d_ptr->m_isQuit)
    {

        if ((unsigned int)index == m_allEvent.size())
            index = 0;

        if (m_allEvent.size() == 0)
            continue;

        XEvent *event = m_allEvent.at(index);

        //执行除事件循环外的事件
        if (event->getEventType() == XEvent::E_XEventLoop)
        {
            index++;
            continue;
        }

        event->doWork();
        index++;
    }
}

void XEventLoop::quit()
{
    d_ptr->m_isQuit = true;
}
