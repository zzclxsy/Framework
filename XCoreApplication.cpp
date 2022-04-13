#include "XCoreApplication.h"
#include "./XEvent/XEvent.h"
#include <string>
#include <mutex>

class XCoreApplicationPrivate
{
public:
    std::vector<XEvent *> m_allEvent;
    bool m_isQuit;
    std::string m_filePath;
    std::string m_fileName;
    std::vector<std::string> m_allParam;
    std::mutex m_mutex;
    XCoreApplication *q_ptr;
};

static XCoreApplication *app;
XCoreApplication::XCoreApplication(int argc, char **argv)
{
    app = this;
    d_ptr = new XCoreApplicationPrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_isQuit = false;

}

void XCoreApplication::exec()
{
    static int index = 0;
    while(!d_ptr->m_isQuit)
    {
        if ((unsigned int)index == d_ptr->m_allEvent.size())
            index = 0;

        if (d_ptr->m_allEvent.size() == 0)
            continue;

        XEvent *event = d_ptr->m_allEvent.at(index);
        event->doWork();
        index++;
    }
}

void XCoreApplication::quit()
{
    d_ptr->m_isQuit = true;
}

void XCoreApplication::addEvent(XEvent *event)
{
    d_ptr->m_mutex.lock();
    d_ptr->m_allEvent.push_back(event);
    d_ptr->m_mutex.unlock();
}

void XCoreApplication::removeEvent(XEvent *event)
{
    d_ptr->m_mutex.lock();
    auto it = d_ptr->m_allEvent.begin();
    while (it != d_ptr->m_allEvent.end())
    {
        if ((*it) == event)
        {
            d_ptr->m_allEvent.erase(it);
            return;
        }
        it++;
    }
    d_ptr->m_mutex.unlock();
}

std::vector<XEvent *> XCoreApplication::getAllEvent()
{
    return d_ptr->m_allEvent;
}

XCoreApplication *XCoreApplication::instant()
{
    return app;
}
