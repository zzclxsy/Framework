#ifndef XMQTTSERVER_H
#define XMQTTSERVER_H

#include "XServer.h"
#include "XApi/VXMQTTServer.h"
#include "XApi/VXFactory.h"
#include "XApi/VXModuleTypeDef.h"
#include "MQTTClient.h"
#include "MQTTAsync.h"
#include <set>
class XMQTTServer:public XServer
        ,public VXMQTTServer
{
public:
    XMQTTServer();
    virtual ~XMQTTServer();
    enum ConnectState{
        Unconnect = 0,
        Connected = 1,
        Connecting = 2
    };

    virtual bool Initialize(const Json::Value& cfgData);
    virtual void Release();

    virtual void addTopic(const std::string &topic, DataCallback fun);
    virtual void removeTopic(const std::string &topic);

    virtual int publish(const std::string &topic, const std::string &payload, int qos);

protected:
    void WorkerProc();

    int CheckConnection();
    int SubscribeTopics();
    int UnsubscribeTopics();
    void DispatchMessage(const std::string &topic, const std::string &payload);
    void RecoverSubTopics();

    static void onConnected(void* ctx, MQTTAsync_successData* response);
    static void onConnectFailed(void* ctx, MQTTAsync_failureData* response);
    static void onConnectionLost(void * ctx, char * cause);
    static void onSubscribe(void* ctx, MQTTAsync_successData* response);
    static void onSubscribeFailed(void* ctx, MQTTAsync_failureData* response);
    static void onUnsubscribe(void* ctx, MQTTAsync_successData* response);
    static void onUnsubscribeFailed(void* ctx, MQTTAsync_failureData* response);
    static void onMessageSent(void* ctx, MQTTAsync_successData* response);
    static void onSendMessageFailed(void* ctx, MQTTAsync_failureData* response);
    static int onMessageReceived(void *ctx, char *topicName, int topicLen, MQTTAsync_message *message);

protected:
    std::thread *m_worker;
    bool m_running;

    MQTTAsync m_client;
    int m_qos;

    std::string m_clientId;
    std::string m_mqttServer;
    std::string m_username;
    std::string m_password;

    int m_connectState;
    int m_connectingCheckTimes;

    std::set<std::string> m_topics;
    std::set<std::string> m_topicsAdd;
    std::set<std::string> m_topicsRemove;
    std::set<std::string> m_topicsSub;
    std::set<std::string> m_topicsUnsub;
    std::mutex m_topicLock;

    std::map<std::string, DataCallback> m_callbacks;
    std::mutex m_cbLock;
};

X_FACTORY_DECL(XMQTTServer)
#endif // XMQTTSERVER_H
