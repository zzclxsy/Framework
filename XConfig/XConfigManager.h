#ifndef XCONFIGMANAGER_H
#define XCONFIGMANAGER_H
#include "XApi/VXModule.h"
#include "XApi/VXModuleConfig.h"
#include "XApi/VXFactory.h"
class XConfigManager:
        public VXModule,
        public VXModuleConfig
{
public:
    XConfigManager();
    virtual ~XConfigManager(){};

    virtual Json::Value loadJson(const std::string& file) override;
    virtual bool loadJsonAs(const std::string &file, const std::string& cfgName) override;
    virtual bool loadIniAs(const std::string &file, const std::string& cfgName) override;
    //
    virtual Json::Value getConfigData(const std::string &cfgName) override;
    virtual std::string getItemAsString(const std::string& cfgName, const std::string& itemName) override;
    virtual int getItemAsInt(const std::string& cfgName, const std::string& itemName) override;
    virtual Json::Value getItemData(const std::string& cfgName, const std::string& itemName) override;
    //
    virtual void addConfigData(const std::string &cfgName, const Json::Value &cfgData) override;
private:
    void recordData(const std::string &cfgName, const Json::Value &cfgData);

protected:
    std::map<std::string, Json::Value> m_configStore;
};
X_FACTORY_DECL(XConfigManager)
#endif // XCONFIGMANAGER_H
