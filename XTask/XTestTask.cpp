#include "XTestTask.h"
#include "XTcpTaskManager.h"
#include "XTaskKeys.h"
#include "XNetwork/XSocketBase.h"
#include "XUtils.h"
XTestTask::XTestTask(const Json::Value& task, XTcpTaskManager * m)
{
    mp_taskManager = m;

    m_taskId = task[TASK_KEY_ID].asString();
    m_data = task;
    m_taskName = task[TASK_KEY_NAME].asString();
}

void XTestTask::TaskDataUpdata(const Json::Value &task)
{
    m_data = task;
}

void XTestTask::SetTaskClient(VXSocketSession *client)
{
    mp_client = client;
}

void XTestTask::StartTask()
{
    std::string taskId = this->GetId();
    Json::Value respData;
    respData[TASK_KEY_ID] = taskId;
    respData[RRPLY_TASK_TYPE] = task_start;
    std::string buffer = XUtils::JsonToString(respData);
    mp_client->SendDataAsync(buffer.c_str(), (int)buffer.size());
}

const std::string &XTestTask::GetTaskName()
{
    return m_taskName;
}

const std::string &XTestTask::GetId()
{
    return m_taskId;
}

const Json::Value &XTestTask::GetData()
{
    return m_data;
}

std::string XTestTask::GetStringProperty(const std::string &name)
{
    return m_data[name].asString();
}

int XTestTask::GetIntProperty(const std::string &name)
{
    return m_data[name].asInt();
}

void XTestTask::TaskUpdate(const Json::Value &data)
{
    Json::Value respData;

    respData[TASK_KEY_ID] = m_taskId;
    respData[RRPLY_TASK_TYPE] = task_updata;
    respData[RRPLY_TASK_RESULT] = data;

    std::string respBuffer = XUtils::JsonToString(respData);
    mp_client->SendDataAsync(respBuffer.c_str(), (int)respBuffer.size());
}

void XTestTask::TaskFinished(const Json::Value &data)
{
    Json::Value respData;

    respData[TASK_KEY_ID] = m_taskId;
    respData[RRPLY_TASK_TYPE] = task_finished;
    respData[RRPLY_TASK_RESULT] = data;

    std::string respBuffer = XUtils::JsonToString(respData);
    mp_client->SendDataAsync(respBuffer.c_str(), (int)respBuffer.size());
}

void XTestTask::TaskErrorAbort(const Json::Value &data)
{
    Json::Value respData;

    respData[TASK_KEY_ID] = m_taskId;
    respData[RRPLY_TASK_TYPE] = task_burst_error;
    respData[RRPLY_TASK_ERROR] = data;

    std::string respBuffer = XUtils::JsonToString(respData);
    mp_client->SendDataAsync(respBuffer.c_str(), (int)respBuffer.size());
}
