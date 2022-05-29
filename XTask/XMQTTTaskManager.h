#ifndef XMQTTTASKMANAGER_H
#define XMQTTTASKMANAGER_H
#include "XApi/VXFactory.h"
#include "XApi/VXTaskManager.h"
#include "XTask/XTestTask.h"
#include "XApi/VXModuleTypeDef.h"
#include <mutex>
#include "XApi/VXMQTTServer.h"

class XMQTTTaskManager:public VXModule,
        public VXTaskManager
{
public:
    XMQTTTaskManager();

    typedef std::shared_ptr<XTestTask> testTaskPtr;

    virtual bool Initialize(const Json::Value& = NULL);
    virtual void Release(){};
    virtual VXTestTask *FindTask(const std::string& taskId);
    virtual void ClearTasks();

protected:
    bool ClearCurrentTask(std::string taskid);
    int TaskHandler(const Json::Value& reqData);
    int DispatchTask(const Json::Value& task);

private:
    VXMQTTServer *mp_mqttServer;
    std::string m_bindIp;
    int m_port;
    testTaskPtr m_currTestTask;
    std::map<std::string, testTaskPtr> m_testTasks;//key:taskID,value:taskclass
    std::mutex m_lock;
};
X_FACTORY_DECL(XMQTTTaskManager)
#endif // XMQTTTASKMANAGER_H
