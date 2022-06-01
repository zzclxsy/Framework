#include "XMQTTServer.h"
#include "XLogPrint/XLogPrint.h"
#include <functional>
#include <assert.h>
#include "XUtils.h"
X_FACTORY_IMPL(XMQTTServer, MODULE_MQTTSERVER,"XMQTTServer")


XMQTTServer::XMQTTServer()
{
    m_worker = nullptr;
    m_running = false;
    m_client = nullptr;
    m_qos = 1;
    m_connectState = XMQTTServer::Unconnect;
    m_connectingCheckTimes = 3;
}

XMQTTServer::~XMQTTServer()
{

}

bool XMQTTServer::Initialize(const Json::Value &cfgData)
{
    if (m_worker){
        return m_running;
    }

    if (cfgData.isMember(MQTT_CLIENT)){
        m_clientId = cfgData[MQTT_CLIENT].asString();
    }else{
        XERROR << "XMQTTServer::Initialize, missing cfg data:" << MQTT_CLIENT;
    }
    if (cfgData.isMember(MQTT_SERVER)){
        m_mqttServer = cfgData[MQTT_SERVER].asString();
    }else{
        XERROR << "JHMqttClient::Initialize, missing cfg data:" << MQTT_SERVER;
    }
    if (cfgData.isMember(MQTT_USERNAME)){
        m_username = cfgData[MQTT_USERNAME].asString();
    }
    if (cfgData.isMember(MQTT_PASSWORD)){
        m_password = cfgData[MQTT_PASSWORD].asString();
    }

    m_running = true;
    m_worker = new std::thread(std::bind(&XMQTTServer::WorkerProc, this));
    return m_running;
}

void XMQTTServer::Release()
{
    if (m_running && m_worker){
        m_running = false;
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
}

void XMQTTServer::addTopic(const std::string &topic, DataCallback fun)
{
    m_topicLock.lock();
    m_topicsAdd.insert(topic);
    auto rmIter = m_topicsRemove.find(topic);
    if (rmIter != m_topicsRemove.end())
    {
        m_topicsRemove.erase(rmIter);
    }
    m_topicLock.unlock();

    m_cbLock.lock();
    auto cbIter = m_callbacks.find(topic);
    if (cbIter == m_callbacks.end())
    {
        m_callbacks[topic] = fun;
    }
    else
    {
        XERROR << "JHMqttClient::addTopic, add topic failed\t" << topic;
    }
    m_cbLock.unlock();
}

void XMQTTServer::removeTopic(const std::string &topic)
{
    m_topicLock.lock();
    m_topicsRemove.insert(topic);
    auto adIter = m_topicsAdd.find(topic);
    if (adIter != m_topicsAdd.end())
    {
        m_topicsAdd.erase(adIter);
    }
    m_topicLock.unlock();

    m_cbLock.lock();
    auto cbIter = m_callbacks.find(topic);
    if (cbIter != m_callbacks.end())
    {
        m_callbacks.erase(cbIter);
    }
    m_cbLock.unlock();
}

int XMQTTServer::publish(const std::string &topic, const std::string &payload, int qos)
{
    if (m_connectState != Connected){
        XERROR << "XMQTTServer::publish, mqtt not connected, failed." << "topic:" << topic << "payload:" << payload  << "onnectState:" << m_connectState;
        return -1;
    }

    MQTTAsync_message message = MQTTAsync_message_initializer;
    message.payload = (void *)payload.data();
    message.payloadlen = (int)payload.length();
    message.qos = qos;
    message.retained = 0;

    MQTTAsync_responseOptions options = MQTTAsync_responseOptions_initializer;
    options.onSuccess = &XMQTTServer::onMessageSent;
    options.onFailure = &XMQTTServer::onSendMessageFailed;
    options.context = this;

    return MQTTAsync_sendMessage(m_client, topic.data(), &message, &options);
}

void XMQTTServer::WorkerProc()
{
    if (m_mqttServer.empty() || m_clientId.empty()){
        XERROR << "XMQTTServer::WorkerProc, error init param.";
        return;
    }

    if (!m_client)
    {
        MQTTAsync_create(
                    &m_client,
                    m_mqttServer.data(),
                    m_clientId.data(),
                    MQTTCLIENT_PERSISTENCE_NONE,
                    nullptr);
        MQTTAsync_setCallbacks(
                    m_client,
                    this,
                    &XMQTTServer::onConnectionLost,
                    &XMQTTServer::onMessageReceived,
                    nullptr);
    }

    if (!m_client)
    {
        XERROR << "HMqttClientAsync::WorkerProc, create mqtt client failed.";
        return;
    }

    this->CheckConnection();
    this->SetTimer(1000, std::bind(&XMQTTServer::CheckConnection, this));
    this->SetTimer(100, std::bind(&XMQTTServer::SubscribeTopics, this));
    this->SetTimer(100, std::bind(&XMQTTServer::UnsubscribeTopics, this));

//    this->SetTimer(10000, [this]()->int{
//        JH_DEBUG() << "topics:" << this->m_topics;
//        return 0;
//    });

    while (m_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        this->ProcessTimers();
    }
}

int XMQTTServer::CheckConnection()
{
    assert(m_client);
    // check connect
    if (MQTTAsync_isConnected(m_client))
    {
        m_connectState = Connected;
        return 0;
    }

    XERROR<<"XMQTTServer::CheckConnection MQTT CONNECTED BUT CURRENT STATUS : " << m_connectState;
    if (m_connectState == Connecting && m_connectingCheckTimes < 5)
    {
        // skipped if connected or conntecting
        ++m_connectingCheckTimes;
        return 0;
    }

    MQTTAsync_connectOptions options = MQTTAsync_connectOptions_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.onSuccess = &XMQTTServer::onConnected;
    options.onFailure = &XMQTTServer::onConnectFailed;
    options.context = this;
    if (!m_username.empty() && !m_password.empty())
    {
        options.username = m_username.c_str();
        options.password = m_password.c_str();
    }
    options.connectTimeout = 1;

    int rc = MQTTAsync_connect(m_client, &options);
    XDEBUG<<" XMQTTServer::CheckConnection MQTT TRY TO CONNECT " << m_clientId;
    if (rc == MQTTASYNC_SUCCESS){
        m_connectState = Connecting;
    }
    m_connectingCheckTimes = 0;
    return rc;
}

int XMQTTServer::SubscribeTopics()
{
    if (!m_topicsSub.empty())
    {
        // maybe some topics are subscribing
        return 0;
    }

    std::string topic;

    m_topicLock.lock();
    while (!m_topicsAdd.empty())
    {
        auto it = m_topicsAdd.begin();
        topic = *it;
        if (m_topics.find(topic) != m_topics.end())
        {
            m_topicsAdd.erase(topic);
            continue;
        }
        break;
    }
    m_topicLock.unlock();

    if (topic.empty())
    {
        // no topic need to add
        return 0;
    }

    MQTTAsync_responseOptions options = MQTTAsync_responseOptions_initializer;
    options.onSuccess = &XMQTTServer::onSubscribe;
    options.onFailure = &XMQTTServer::onSubscribeFailed;
    options.context = this;

    assert(m_client);
    int rc = MQTTAsync_subscribe(m_client, topic.c_str(), 1, &options);

    m_topicLock.lock();
    if (rc == MQTTASYNC_SUCCESS)
    {
        m_topicsSub.insert(topic);
        m_topicsAdd.erase(topic);
        m_topics.insert(topic);
    }
    m_topicLock.unlock();

    return rc;
}

int XMQTTServer::UnsubscribeTopics()
{
    if (!m_topicsUnsub.empty())
    {
        // maybe some topics are unsubscribing
        return 0;
    }

    std::string topic;

    m_topicLock.lock();
    if (!m_topicsRemove.empty())
    {
        auto it = m_topicsRemove.begin();
        topic = *it;
    }
    m_topicLock.unlock();

    if (topic.empty())
    {
        // no topic need to remove
        return 0;
    }

    MQTTAsync_responseOptions options = MQTTAsync_responseOptions_initializer;
    options.onSuccess = &XMQTTServer::onUnsubscribe;
    options.onFailure = &XMQTTServer::onSubscribeFailed;
    options.context = this;

    int rc = MQTTAsync_unsubscribe(m_client, topic.data(), &options);
    m_topicLock.lock();
    if (rc == MQTTASYNC_SUCCESS)
    {
        m_topicsUnsub.insert(topic);
        auto it = m_topics.find(topic);
        if (it != m_topics.end())
        {
            m_topics.erase(it);
        }

        m_topicsRemove.erase(topic);
    }
    m_topicLock.unlock();

    return rc;
}

void XMQTTServer::DispatchMessage(const std::string &topic, const std::string &payload)
{
    XDEBUG << "XMQTTServer::DispatchMessage:\t" << topic;

    m_cbLock.lock();
    DataCallback callback;
    auto it = m_callbacks.find(topic);
    if (it != m_callbacks.end())
    {
        callback = it->second;
    }
    m_cbLock.unlock();

    if (callback)
    {
        callback(payload);
    }
}

void XMQTTServer::RecoverSubTopics()
{
    XDEBUG << "XMQTTServer::RecoverSubTopics" << m_clientId;
    m_topicLock.lock();
    //m_topicsAdd = m_topics;
    for (auto topic : m_topics)
    {
        m_topicsAdd.insert(topic);
    }
    for (auto topic : m_topicsSub)
    {
        m_topicsAdd.insert(topic);
    }
    m_topics.clear();
    m_topicsSub.clear();
    m_topicsRemove.clear();
    m_topicsUnsub.clear();
    m_topicLock.unlock();
}

void XMQTTServer::onConnected(void *ctx, MQTTAsync_successData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_connectState = XMQTTServer::Connected;
    XDEBUG << "XMQTTServer::onConnected" << mqttClient->m_clientId;
    mqttClient->RecoverSubTopics();
}

void XMQTTServer::onConnectFailed(void *ctx, MQTTAsync_failureData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_connectState = XMQTTServer::Unconnect;

    XERROR << "XMQTTServer::onConnectFailed" << mqttClient->m_clientId;
}

void XMQTTServer::onConnectionLost(void *ctx, char *cause)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_connectState = XMQTTServer::Unconnect;
    mqttClient->RecoverSubTopics();

    XERROR << "XMQTTServer::onConnectionLost." << mqttClient->m_clientId;
}

void XMQTTServer::onSubscribe(void * ctx, MQTTAsync_successData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_topicLock.lock();
    mqttClient->m_topicsSub.clear();
    mqttClient->m_topicLock.unlock();

    XDEBUG << "XMQTTServer::onSubscribe" << mqttClient->m_clientId;
}

void XMQTTServer::onSubscribeFailed(void * ctx, MQTTAsync_failureData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_topicLock.lock();
    auto it = mqttClient->m_topicsSub.begin();
    for (; it != mqttClient->m_topicsSub.end(); ++it)
    {
        mqttClient->m_topicsAdd.insert(*it);
    }
    mqttClient->m_topicsSub.clear();
    mqttClient->m_topicLock.unlock();

    XERROR << "XMQTTServer::onSubscribeFailed." << mqttClient->m_clientId;
}

void XMQTTServer::onUnsubscribe(void *ctx, MQTTAsync_successData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_topicLock.lock();
    mqttClient->m_topicsUnsub.clear();
    mqttClient->m_topicLock.unlock();

    XDEBUG << "XMQTTServer::onUnsubscribe." << mqttClient->m_clientId;
}

void XMQTTServer::onUnsubscribeFailed(void *ctx, MQTTAsync_failureData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    mqttClient->m_topicLock.lock();
    auto it = mqttClient->m_topicsUnsub.begin();
    for (; it != mqttClient->m_topicsUnsub.end(); ++it)
    {
        mqttClient->m_topicsRemove.insert(*it);
    }
    mqttClient->m_topicsUnsub.clear();
    mqttClient->m_topicLock.unlock();

    XERROR << "XMQTTServer::onUnsubscribeFailed." << mqttClient->m_clientId;
}

void XMQTTServer::onMessageSent(void * ctx, MQTTAsync_successData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

//    JH_DEBUG() << "XMQTTServer::onMessageSent" << mqttClient->m_clientId;
}

void XMQTTServer::onSendMessageFailed(void *ctx, MQTTAsync_failureData *response)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;
    assert(mqttClient);

    XERROR << "XMQTTServer::onSendMessageFailed" << mqttClient->m_clientId << response->message;
}

int XMQTTServer::onMessageReceived(void *ctx, char *topicName, int topicLen, MQTTAsync_message *message)
{
    XMQTTServer * mqttClient = (XMQTTServer *)ctx;

    std::string topic = std::string(topicName);
    std::string payload = std::string((char *)message->payload, message->payloadlen);

    // dispatch messages here
    mqttClient->DispatchMessage(topic, payload);

    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);

    return 1;
}
