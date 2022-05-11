#include "XCoreApplication.h"
#include "./XEvent/XEvent.h"
#include "XLogPrint/XLogPrint.h"
#include "XTime/XTime.h"
#include "XObjectManager.h"
#include "XConfig/XConfigManager.h"
#include "XNetwork/XTcpClient.h"
#include "XNetwork/XTcpServer.h"
#include "XTask/XTcpTaskManager.h"
#include <assert.h>
#include <string>
#include <mutex>
#include "XApi/VXApi.h"
#include <thread>
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
    XObjectManager *mp_objectManager;
    std::string m_appPath;
    std::string m_appName;
    std::vector<std::string> m_arguments;
};

static XCoreApplication *app;
XCoreApplication::XCoreApplication(int argc, char **argv)
{
    app = this;
    d_ptr = new XCoreApplicationPrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_isQuit = false;
    d_ptr->mp_objectManager = new XObjectManager;

    for (int i = 0; i < argc; ++i){
        d_ptr->m_arguments.push_back(std::string(argv[i]));
    }
    assert(d_ptr->m_arguments.size() > 0);

    std::string fullPath = d_ptr->m_arguments.at(0);
    char * p = const_cast<char *>(fullPath.c_str());
    for (size_t i = 0; i != fullPath.size(); ++i){
        if (p[i] == '\\'){
            p[i] = '/';
        }
    }
    if (fullPath.find_last_of('/') == std::string::npos)
    {
        d_ptr->m_appPath = "./";
        d_ptr->m_appName = fullPath;
    }
    else
    {
        d_ptr->m_appPath = fullPath.substr(0, fullPath.find_last_of('/'));
        d_ptr->m_appName = fullPath.substr(fullPath.find_last_of('/') + 1);
    }
    //注册时间模块
	this->Register(new X_FACTORY_NAME(XTime));
    //注册配置管理模块
    this->Register(new X_FACTORY_NAME(XConfigManager));
    //注册TcpTask管理模块
    this->Register(new X_FACTORY_NAME(XTcpTaskManager));
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

        //防止事件被移除，导致索引溢出
        if (d_ptr->m_mutex.try_lock() == false)
            continue;
        
        XEvent *event = d_ptr->m_allEvent.at(index);
        event->doWork();
        d_ptr->m_mutex.unlock();
        index++;
    }
}

void XCoreApplication::quit()
{
    d_ptr->m_isQuit = true;
}


std::shared_ptr<VXTcpClient> XCoreApplication::CreateTcpClient()
{
    return std::make_shared<XTcpClient>();
}

std::shared_ptr<VXTcpServer> XCoreApplication::CreateTcpServer()
{
    return std::make_shared<XTcpServer>();
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
            break;
        }
        it++;
    }
    d_ptr->m_mutex.unlock();
}

std::vector<XEvent *> XCoreApplication::getAllEvent()
{
    return d_ptr->m_allEvent;
}

XCoreApplication *XCoreApplication::GetApp()
{
    return app;
}

int XCoreApplication::Register(VXFactory *factory)
{
    return d_ptr->mp_objectManager->Register(factory);
}

VXModule *XCoreApplication::GetModule(int id)
{
    return d_ptr->mp_objectManager->GetModule(id);
}

std::string XCoreApplication::GetAppPath()
{
    return d_ptr->m_filePath;
}

std::string XCoreApplication::GetAppName()
{
    return d_ptr->m_appName;
}

std::vector<std::string> XCoreApplication::GetArguments()
{
    return d_ptr->m_arguments;
}

//------------------------------------------模块获取----------------------------------------------------
VXTime * XGetTimeModule()
{
    return XCoreApplication::GetXService<VXTime>(MODULE_TIME_API);
}

VXModuleConfig *XGetConfigModule()
{
    return XCoreApplication::GetXService<VXModuleConfig>(MODULE_CONFIG_ID);
}
VXModuleTest *XGetPGMTest()
{
    return XCoreApplication::GetXService<VXModuleTest>(MODULE_TEST_PGM_ID);
}
VXTaskManager *XGetTcpTaskManager()
{
    return XCoreApplication::GetXService<VXTaskManager>(MODULE_TASKMANAGER);
}
