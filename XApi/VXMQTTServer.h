#ifndef VXMQTTSERVER_H
#define VXMQTTSERVER_H
#include <string>
#include <functional>
#include "json/json.h"
class VXMQTTServer
{
public:
    typedef std::function<void (const std::string&)> DataCallback;

    virtual void addTopic(const std::string &topic, DataCallback fun) = 0;
    virtual void removeTopic(const std::string &topic) = 0;
    virtual int publish(const std::string &topic, const std::string &payload, int qos) = 0;
};


#define MAX_CLIENT_ID_LEN 20
#define MAX_TOPICS     128
#define MAX_PUB_MESSAGE_COUNT 1000
#define MAX_PUB_MESSAGE_SEND_TIMES 3
#define MAX_RECV_MSG_TIMEOUTMS 30
#define MQTT_RECONNET_BASE_TIMEMS 100
#define MQTT_RECONNET_MAX_COUNT 30

const std::string MQTT_CLIENT = "clientId";
const std::string MQTT_SERVER = "mqttServer";
const std::string MQTT_USERNAME = "username";
const std::string MQTT_PASSWORD = "password";

#endif // VXMQTTSERVER_H
