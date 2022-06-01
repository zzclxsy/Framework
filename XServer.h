#ifndef XSERVER_H
#define XSERVER_H

#include "XApi/VXModule.h"
#include <functional>
#include <thread>
#include <mutex>

class XServer:public VXModule
{
public:
    XServer();
    virtual ~XServer();

    virtual bool Initialize(const Json::Value& /*cfgData*/);
    virtual void Release();

protected:
    virtual void WorkerProc();

    int SetTimer(int interval, std::function<int ()> callback);
    void RemoveTimer(int id);
    void ProcessTimers();

protected:
    std::thread * m_worker;
    bool m_running;

    struct Timer{
        int id;
        long long interval;
        std::function<int ()> callback;
        long long lastTriggered;
    };
    std::map<int, Timer> m_timers;
};

#endif // XSERVER_H
