#include "XMQTTTaskManager.h"
#include "XCoreApplication.h"
#include "XMQTT/XMQTTServer.h"
#include "XLogPrint/XLogPrint.h"
#include "XUtils.h"
#include "XTaskKeys.h"
X_FACTORY_IMPL(XMQTTTaskManager, MODULE_MQTTTASK, "XMQTTTaskManager")

XMQTTTaskManager::XMQTTTaskManager()
{
    X_MODULE_INIT(XMQTTTaskManager)
}

bool XMQTTTaskManager::Initialize(const Json::Value &)
{
    //判断是否注册mqtt服务
    mp_mqttServer = XCoreApplication::GetApp()->GetXService<VXMQTTServer>(MODULE_MQTTSERVER);
    if (mp_mqttServer == nullptr)
    {
        XERROR <<"XMQTTTaskManager::Initialize: not mqtt server";
        return false;
    }

    mp_mqttServer->addTopic("MQTT Examples",std::bind(&XMQTTTaskManager::TaskHandler,this,std::placeholders::_1));
    return true;
}

VXTestTask *XMQTTTaskManager::FindTask(const std::string &taskId)
{
    return nullptr;
}

void XMQTTTaskManager::ClearTasks()
{

}

bool XMQTTTaskManager::ClearCurrentTask(std::string taskid)
{
    return false;
}

int XMQTTTaskManager::TaskHandler(const Json::Value &reqData)
{
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
       // return session->SendData(buffer.c_str(), (int)buffer.size());
    };

    if (error != "")//包格式错误
    {
       // return errorSend(error);
    }

    if (reqData[REC_TASK_STATUS].asString() == "clear")
    {
        if (this->ClearCurrentTask(reqData[TASK_KEY_ID].asString()) == false)
         ;//  return errorSend("The clear object could not be found!!!");

        return 0;
    }

    return this->DispatchTask(reqData);
}

int XMQTTTaskManager::DispatchTask(const Json::Value &task)
{
    XDEBUG << XUtils::JsonToString(task);
    return 0;
}
