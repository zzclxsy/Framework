#ifndef XTESTTASK_H
#define XTESTTASK_H
#include "XApi/VXTestTask.h"

class XSocketSession;
class XTcpTaskManager;
class XTestTask:public VXTestTask
{
public:
    XTestTask(const Json::Value& task, XTcpTaskManager * m);

    void TaskDataUpdata(const Json::Value& task);
    void SetTaskClient(XSocketSession *client);
    void StartTask();

    virtual const std::string &GetId();
    virtual const Json::Value &GetData();
    virtual std::string GetStringProperty(const std::string& name);
    virtual int GetIntProperty(const std::string& name);
    virtual void TaskUpdate(const Json::Value& data);
    virtual void TaskFinished(const Json::Value& data);

private:
    XTcpTaskManager *mp_taskManager;
    std::string m_taskId;
    Json::Value m_data;
    XSocketSession *mp_client;
};

#endif // XTESTTASK_H
