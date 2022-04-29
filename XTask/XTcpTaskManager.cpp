#include "XTcpTaskManager.h"
#include "XLogPrint/XLogPrint.h"
#include "XUtils.h"
#include "XTaskKeys.h"
#include "XApi/VXApi.h"
#include <mutex>
XTcpTaskManager::XTcpTaskManager()
{

}

bool XTcpTaskManager::Initialize(const Json::Value & cfgData)
{
    XINFO << "JHUdpTaskManager::Initialize";

    if (m_tcpServer.use_count() != 0)
    {
        return true;
    }

    if (cfgData.isMember("ip"))
    {
        m_bindIp = cfgData["ip"].asString();
    }
    else
    {
        XERROR << "JHUdpTaskManager::Initialize, missing cfg data:" << "ip";
        return false;
    }
    if (cfgData.isMember("port"))
    {
        m_port = cfgData["port"].asInt();
    }
    else
    {
        XERROR << "JHUdpTaskManager::Initialize, missing cfg data:" << "port";
        return false;
    }

    m_tcpServer = std::make_shared<XTcpServer>();
    m_tcpServer->SetIpAddress(m_bindIp);
    m_tcpServer->SetPort(m_port);
    m_tcpServer->SetDataHandler(std::bind(&XTcpTaskManager::TaskHandler,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2,
                                std::placeholders::_3));

    m_tcpServer->Start();

    return true;
}

void XTcpTaskManager::Release()
{
    m_tcpServer->Stop();
}

VXTestTask *XTcpTaskManager::FindTask(const std::string &taskId)
{
    return nullptr;
}

void XTcpTaskManager::ClearTasks()
{

}

void XTcpTaskManager::TaskFinished()
{

}

void XTcpTaskManager::ClearCurrentTask()
{

}

int XTcpTaskManager::TaskHandler(XSocketSession *session, const char *data, int length)
{
    Json::Value reqData = XUtils::StringToJson(data);

    Json::Value respData;
    respData[TASK_KEY_STATUS] = "ok";
    std::string buffer = XUtils::JsonToString(respData);

    std::string TASK_NAME_KEY = "taskName";
    if (!reqData.isMember(TASK_NAME_KEY))
    {
        XERROR << "XTcpTaskManager::TaskHandler, error task detected, no task name.";
        return session->SendData(buffer.c_str(), (int)buffer.size());
    }
    if (reqData[TASK_NAME_KEY].asString() == "clear")
    {
        this->ClearCurrentTask();
        return 0;
    }
    return this->DispatchTask(session, reqData);
}

int XTcpTaskManager::DispatchTask(XSocketSession *session, const Json::Value &task)
{
    VXModuleTest *PGM = XGetPGMTest();
    if (PGM == nullptr)
    {
        XERROR <<"PGM is no load";
        return -1;
    }
    std::string key = task[TASK_KEY_ID].asString();
    if (m_currTestTask.use_count() != 0 && m_currTestTask->GetId() == key)
    {
        m_currTestTask->TaskDataUpdata(task);
        PGM->ExecuteTask(m_currTestTask.get());
        return 0;
    }

    //判断是否是以前的任务
     auto it = m_testTasks.find(key);
     if (it != m_testTasks.end())
     {
         m_currTestTask = it->second;
         m_currTestTask->TaskDataUpdata(task);
         PGM->ExecuteTask(m_currTestTask.get());
         return 0;
     }

     //创建新任务

    m_currTestTask = std::make_shared<XTestTask>(task, this);

}
