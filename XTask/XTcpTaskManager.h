#ifndef XTCPTASKMANAGER_H
#define XTCPTASKMANAGER_H
#include "XApi/VXFactory.h"
#include "XApi/VXTaskManager.h"
#include "XNetwork/XTcpServer.h"
#include "XTask/XTestTask.h"
#include <mutex>
class XTcpTaskManager:
        public VXModule,
        public VXTaskManager
{
    typedef std::shared_ptr<XTestTask> testTaskPtr;
public:
    XTcpTaskManager();

    virtual bool Initialize(const Json::Value&);
    virtual void Release();
    virtual VXTestTask *FindTask(const std::string& taskId);
    virtual void ClearTasks();

protected:
    bool ClearCurrentTask(std::string taskid);
    int TaskHandler(VXSocketSession * session, const char * data, int length);
    int DispatchTask(VXSocketSession * session, const Json::Value& task);

private:
    std::shared_ptr<XTcpServer> m_tcpServer;
    std::string m_bindIp;
    int m_port;
    testTaskPtr m_currTestTask;
    std::map<std::string, testTaskPtr> m_testTasks;//key:taskID,value:taskclass
    std::mutex m_lock;
};
X_FACTORY_DECL(XTcpTaskManager)

#endif // XTCPTASKMANAGER_H
