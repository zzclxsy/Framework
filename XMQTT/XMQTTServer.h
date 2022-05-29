#ifndef XMQTTSERVER_H
#define XMQTTSERVER_H

#include "XApi/VXModule.h"
#include "XApi/VXMQTTServer.h"
#include "XApi/VXFactory.h"
#include "XApi/VXModuleTypeDef.h"
#include "MQTTClient.h"
class XMQTTServer:public VXModule
        ,public VXMQTTServer
{
public:
    XMQTTServer();
    virtual ~XMQTTServer(){}
    virtual bool Initialize(const Json::Value&);
    virtual void Release();
    virtual bool AddTopical(std::string topical, handle);
    virtual bool RemoveTopical(std::string topical);
private:
    //static void DisConnect(void *context, char *cause);
    //static int Recvice(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    //static void Delivered(void *context, MQTTClient_deliveryToken dt);

private:
    MQTTClient m_client;
    int m_QOS;
};

X_FACTORY_DECL(XMQTTServer)
#endif // XMQTTSERVER_H
