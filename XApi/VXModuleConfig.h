#ifndef VXMODULECONFIG_H
#define VXMODULECONFIG_H
#include <string>
#include <json/json.h>

class VXModuleConfig
{
public:
    virtual Json::Value loadJson(const std::string &file) = 0;
    virtual bool loadJsonAs(const std::string &file, const std::string& cfgName) = 0;
    virtual bool loadIniAs(const std::string &file, const std::string& cfgName) = 0;
    virtual Json::Value getConfigData(const std::string &cfgName) = 0;
    virtual std::string getItemAsString(const std::string& cfgName, const std::string& itemName) = 0;
    virtual int getItemAsInt(const std::string& cfgName, const std::string& itemName) = 0;
    virtual Json::Value getItemData(const std::string& cfgName, const std::string& itemName) = 0;
    virtual void addConfigData(const std::string &cfgName, const Json::Value &cfgData) = 0;
};
#endif // VXMODULECONFIG_H
