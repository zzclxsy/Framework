#include "XTcpTaskManager.h"
#include "XLogPrint/XLogPrint.h"
#include "XUtils.h"
#include "XTaskKeys.h"
#include "XApi/VXApi.h"
#include <mutex>
X_FACTORY_IMPL(XTcpTaskManager, MODULE_TASKMANAGER, "XTcpTaskManager")
XTcpTaskManager::XTcpTaskManager()
{
    X_MODULE_INIT(XTcpTaskManager)
}

bool XTcpTaskManager::Initialize(const Json::Value & cfgData)
{
    XINFO << "XTcpTaskManager::Initialize";

    if (m_tcpServer.use_count() != 0)
    {
        return true;
    }

    if (cfgData.isMember("ListenIP"))
    {
        m_bindIp = cfgData["ListenIP"].asString();
    }
    else
    {
        XERROR << "XTcpTaskManager::Initialize, missing cfg data:" << "ip";
        return false;
    }
    if (cfgData.isMember("BindPort"))
    {
        m_port = cfgData["BindPort"].asInt();
    }
    else
    {
        XERROR << "XTcpTaskManager::Initialize, missing cfg data:" << "port";
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
    auto it = m_testTasks.find(taskId);
    if (it != m_testTasks.end())
    {
        return it->second.get();
    }

    return nullptr;
}

void XTcpTaskManager::ClearTasks()
{
    m_testTasks.clear();
}

bool XTcpTaskManager::ClearCurrentTask(std::string taskid)
{
    auto it = m_testTasks.find(taskid);
    if (it != m_testTasks.end())
    {
        m_testTasks.erase(it);
        return true;
    }
    XERROR << "XTcpTaskManager::ClearCurrentTask, The clear object could not be found!!!";
    return false;
}

int XTcpTaskManager::TaskHandler(VXSocketSession *session, const char *data, int length)
{
    static char buf[100];
    memset(buf, 0, 100);
    memcpy(buf, data, length);
    Json::Value reqData = XUtils::StringToJson(buf);

    std::string error = "";
    std::string taskId, taskNmae;

    if (reqData.isMember(TASK_KEY_NAME) == false)
    {
        XERROR << "XTcpTaskManager::TaskHandler, error task detected, no task name.";
        taskNmae = no_data;
        error += "no task name\n";
    }
    if (reqData.isMember(TASK_KEY_ID) == false)
    {
        XERROR << "XTcpTaskManager::TaskHandler: no taskid, Unable to clear task";
        taskId = no_data;
        error += "no taskid, Unable to clear task\n";
    }
    if (reqData.isMember(REC_TASK_STATUS) == false)
    {
        XERROR << "XTcpTaskManager::TaskHandler: No task status command";
        error += "No task status command\n";
    }

    auto errorSend = [&](std::string err){
        Json::Value respData;
        respData[RRPLY_TASK_TYPE] =task_parse;
        respData[TASK_KEY_ID] = taskId;
        respData[TASK_KEY_NAME] = taskNmae;
        respData[RRPLY_TASK_ERROR] = err;

        std::string buffer = XUtils::JsonToString(respData);
        return session->SendData(buffer.c_str(), (int)buffer.size());
    };

    if (error != "")//包格式错误
    {
        return errorSend(error);
    }

    if (reqData[REC_TASK_STATUS].asString() == "clear")
    {
        if (this->ClearCurrentTask(reqData[TASK_KEY_ID].asString()) == false)
           return errorSend("The clear object could not be found!!!");

        return 0;
    }

    return this->DispatchTask(session, reqData);
}

int XTcpTaskManager::DispatchTask(VXSocketSession *session, const Json::Value &task)
{
    std::lock_guard<std::mutex> lock(m_lock);

    VXModuleTest *PGM = XGetPGMTest();
    if (PGM == nullptr)
    {
        XERROR <<"PGM is no load";
        return -1;
    }
    std::string key = task[TASK_KEY_ID].asString();
    //判断是否是当前任务
    if (m_currTestTask.use_count() != 0 && m_currTestTask->GetId() == key)
    {
        m_currTestTask->SetTaskClient(session);
        m_currTestTask->TaskDataUpdata(task);
        PGM->ExecuteTask(m_currTestTask.get());
        return 0;
    }

    //判断是否是以前的任务
    auto it = m_testTasks.find(key);
    if (it != m_testTasks.end())
    {
        m_currTestTask = it->second;
        m_currTestTask->SetTaskClient(session);
        m_currTestTask->TaskDataUpdata(task);
        m_currTestTask->StartTask();
        PGM->ExecuteTask(m_currTestTask.get());
        return 0;
    }

    //创建新任务
    m_currTestTask = std::make_shared<XTestTask>(task, this);
    m_testTasks.insert(std::pair<std::string, testTaskPtr>(key, m_currTestTask));
    m_currTestTask->SetTaskClient(session);
    m_currTestTask->StartTask();
    PGM->ExecuteTask(m_currTestTask.get());
    return 0;
}
