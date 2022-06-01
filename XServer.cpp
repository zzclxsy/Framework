#include "XServer.h"
#include "XLogPrint/XLogPrint.h"
#include <assert.h>
#include "XApi/VXApi.h"
XServer::XServer()
{
    m_worker = nullptr;
    m_running = false;
}

XServer::~XServer()
{

}

bool XServer::Initialize(const Json::Value& /*cfgData*/)
{
    if (m_worker)
    {
        return m_running;
    }

    m_running = true;
    m_worker = new std::thread([this](){
        try{
            this->WorkerProc();
        }
        catch(std::exception e)
        {
            XERROR << "std::exception from JHService.WorkerProc catched:" << e.what();
        }
        catch(...)
        {
            XERROR << "unhandled exception from JHService.WorkerProc catched.";
        }
    });
    return true;
}

void XServer::Release()
{
    m_running = false;
    if (m_worker)
    {
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
}

void XServer::WorkerProc()
{
    while (m_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        this->ProcessTimers();
    }
}

int XServer::SetTimer(int interval, std::function<int ()> callback)
{
    int tid = 1;
    if (!m_timers.empty()){
        tid = m_timers.rbegin()->first + 1;
    }

    assert(m_timers.find(tid) == m_timers.end());

    Timer t;
    t.id = tid;
    t.interval = interval;
    t.callback = callback;
    t.lastTriggered = 0;

    m_timers.insert(std::make_pair(tid, t));

    return tid;
}

void XServer::RemoveTimer(int id)
{
    auto it = m_timers.find(id);
    if (it != m_timers.end())
    {
        m_timers.erase(it);
    }
}

void XServer::ProcessTimers()
{
    //     JH_DEBUG() << "JHService::ProcessTimers";

    long long current = XGetTimeModule()->getMsecTimestamp();

    std::map<int, Timer>::iterator it = m_timers.begin();
    for (; it != m_timers.end(); ++it)
    {
        long long duration = abs(current - it->second.lastTriggered);

        if (duration < it->second.interval)
        {
            continue;
        }
        if (it->second.callback() == 0)
        {
            it->second.lastTriggered = current;
        }
    }
}
