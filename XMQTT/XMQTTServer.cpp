#include "XMQTTServer.h"
#include "XLogPrint/XLogPrint.h"
#include <functional>
#include "XUtils.h"
X_FACTORY_IMPL(XMQTTServer, MODULE_MQTTSERVER,"XMQTTServer")


void Delivered(void *context, MQTTClient_deliveryToken dt);
void DisConnect(void *, char *cause);
int Recvice(void *context, char *topicName, int topicLen, MQTTClient_message *message);
std::map<std::string, std::function<void (Json::Value &)>> m_callbackMap;
XMQTTServer::XMQTTServer()
{
    X_MODULE_INIT(XMQTTServer)
}

bool XMQTTServer::Initialize(const Json::Value &cfgData)
{
    std::string ServerAddress;
    std::string ClientID;
    if (cfgData.isMember("ServerAddress"))
    {
        ServerAddress = cfgData["ServerAddress"].asString();
    }
    else
    {
        XERROR << "XMQTTServer::Initialize, missing cfg data:" << "ServerAddress";
        return false;
    }

    if (cfgData.isMember("QOS"))
    {
        m_QOS = cfgData["QOS"].asInt();
    }
    else
    {
        XERROR << "XTcpTaskManager::Initialize, missing cfg data:" << "QOS";
        return false;
    }

    if (cfgData.isMember("ClientID"))
    {
        ClientID = cfgData["ClientID"].asString();
    }
    else
    {
        XERROR << "XTcpTaskManager::Initialize, missing cfg data:" << "ClientID";
        return false;
    }


    int rc = 0;
    if ((rc = MQTTClient_create(&m_client, ServerAddress.c_str(), ClientID.c_str(),
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        XERROR<<"Failed to create client, return code:"<< rc;
        return false;
    }

    if ((rc = MQTTClient_setCallbacks(m_client, NULL,DisConnect, Recvice, Delivered)) != MQTTCLIENT_SUCCESS)
    {
        XERROR << "Failed to set callbacks, return code %d\n" << rc;
        return false;
    }

    rc = 0;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(m_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        XERROR <<"Failed to connect, return code %d\n"<< rc;
        return false;
    }

    return true;
}

void XMQTTServer::Release()
{

}

bool XMQTTServer::AddTopical(std::string topical, VXMQTTServer::handle datahanle)
{
    int rc = 0;
    if ((rc = MQTTClient_subscribe(m_client, topical.c_str(), m_QOS)) != MQTTCLIENT_SUCCESS)
    {
        XERROR<<"Failed to subscribe, return code %d\n"<< rc;
        return false;
    }

    XDEBUG << "XMQTTServer::AddTopical";
    m_callbackMap.insert(std::pair<std::string, handle>(topical, datahanle));
    return true;
}

bool XMQTTServer::RemoveTopical(std::string topical)
{
    auto it = m_callbackMap.find(topical);
    if (it == m_callbackMap.end())
        return false;

    m_callbackMap.erase(it);
    return true;
}


int Recvice(void *, char *topicName, int , MQTTClient_message *message)
{
    do
    {
        auto it = m_callbackMap.find(topicName);
        if (it == m_callbackMap.end())
            break;

        Json::Value root;
        if (XUtils::StringToJson((char*)message->payload,message->payloadlen,root) == false)
            break;

        it->second(root);
    }while(0);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

// 主题发布成功回调函数
void Delivered(void *, MQTTClient_deliveryToken dt)
{
    XDEBUG << "XMQTTServer::Delivered :" <<dt;
}

//  断开连接回调函数
void DisConnect(void *, char *cause)
{
    XERROR << "mqtt Connection lost cause:" << cause;
}

