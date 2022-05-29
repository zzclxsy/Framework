#ifndef VXMQTTSERVER_H
#define VXMQTTSERVER_H
#include <string>
#include <functional>
#include "json/json.h"
class VXMQTTServer
{
public:
    typedef std::function<void (Json::Value &)> handle;

    virtual bool AddTopical(std::string topical, handle) = 0;
    virtual bool RemoveTopical(std::string topical) = 0;
};

#endif // VXMQTTSERVER_H
