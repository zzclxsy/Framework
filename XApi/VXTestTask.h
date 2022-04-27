#ifndef VXTESTTASK_H
#define VXTESTTASK_H
#include <string>
#include <json/json.h>
#include <set>
class VXTestTask
{
public:
    virtual const std::string& GetId() = 0;
    virtual const Json::Value& GetData() = 0;
    virtual std::string GetStringProperty(const std::string& name) = 0;
    virtual int GetIntProperty(const std::string& name) = 0;
    virtual void TaskUpdate(const Json::Value& data) = 0;
    virtual void TaskFinished(const Json::Value& data) = 0;
};

#endif // VXTESTTASK_H
