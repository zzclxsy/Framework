#ifndef XTESTTASK_H
#define XTESTTASK_H
#include "XApi/VXTestTask.h"
#include "XApi/VXSocketSession.h"

class XTcpTaskManager;
class XTestTask:public VXTestTask
{
public:
    XTestTask(const Json::Value& task, XTcpTaskManager * m);

    void TaskDataUpdata(const Json::Value& task);
    void SetTaskClient(VXSocketSession *client);
    void StartTask();

    virtual const std::string &GetTaskName();
    virtual const std::string &GetId();
    virtual const Json::Value &GetData();
    virtual std::string GetStringProperty(const std::string& name);
    virtual int GetIntProperty(const std::string& name);
    virtual void TaskUpdate(const Json::Value& data);
    virtual void TaskFinished(const Json::Value& data);
    virtual void TaskErrorAbort(const Json::Value& data);

private:
    XTcpTaskManager *mp_taskManager;
    std::string m_taskId;
    Json::Value m_data;
    std::string m_taskName;
    VXSocketSession *mp_client;
};

#endif // XTESTTASK_H
