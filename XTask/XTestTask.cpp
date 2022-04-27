#include "XTestTask.h"
#include "XTcpTaskManager.h"
#include "XTaskKeys.h"
XTestTask::XTestTask(const Json::Value& task, XTcpTaskManager * m)
{
    mp_taskManager = m;

    m_taskId = task[TASK_KEY_ID].asString();
    m_data = task;
}

void XTestTask::TaskDataUpdata(const Json::Value &task)
{
    m_data = task;
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

}

void XTestTask::TaskFinished(const Json::Value &data)
{

}
