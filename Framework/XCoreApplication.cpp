#include "XCoreApplication.h"
#include "./XEvent/XEvent.h"
static XCoreApplication *app;
XCoreApplication::XCoreApplication(int argc, char **argv)
{
    app = this;
    m_isQuit = false;
}

void XCoreApplication::exec()
{
    static int index = 0;
    while(!m_isQuit)
    {
        if ((unsigned int)index == m_allEvent.size())
            index = 0;

        if (m_allEvent.size() == 0)
            continue;

        XEvent *event = m_allEvent.at(index);
        event->doWork();
        index++;
    }
}

void XCoreApplication::quit()
{
    m_isQuit = true;
}

void XCoreApplication::addEvent(XEvent *event)
{
    m_allEvent.push_back(event);
}

void XCoreApplication::removeEvent(XEvent *event)
{
    auto it = m_allEvent.begin();
    while (it != m_allEvent.end())
    {
        if ((*it) == event)
        {
            m_allEvent.erase(it);
            return;
        }
        it++;
    }
}

XCoreApplication *XCoreApplication::instant()
{
    return app;
}
