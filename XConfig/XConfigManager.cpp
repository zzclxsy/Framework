#include "XConfigManager.h"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>
#include "XLogPrint/XLogPrint.h"
#include "XUtils.h"
#include "XApi/VXModuleTypeDef.h"
X_FACTORY_IMPL(XConfigManager, MODULE_CONFIG_ID, "XConfigManager")
XConfigManager::XConfigManager()
{
    X_MODULE_INIT(XConfigManager)
}

Json::Value XConfigManager::loadJson(const std::string &file)
{
    std::ifstream fileStream;
    fileStream.open(file, std::ifstream::in);
    if (!fileStream.is_open()){
        XERROR << "JHConfigManager::loadJsonAs: open file failed," << file;
        return false;
    }
    std::stringstream fileContent;
    fileContent << fileStream.rdbuf();
    fileStream.close();

    return XUtils::StringToJson(fileContent.str());
}

bool XConfigManager::loadJsonAs(const std::string &file, const std::string &cfgName)
{
    std::ifstream fileStream;
    fileStream.open(file, std::ifstream::in);
    if (!fileStream.is_open()){
        std::cerr << "JHConfigManager::loadJsonAs: open file failed," << file << std::endl;
        return false;
    }
    std::stringstream fileContent;
    fileContent << fileStream.rdbuf();

    Json::Value cfgData = XUtils::StringToJson(fileContent.str());

    fileStream.close();

    if (!cfgData.empty()){
        recordData(cfgName,cfgData);
        return true;
    }
    XERROR << "JHConfigManager::loadJsonAs: file is empty or parse content failed.";
    return false;
}

bool XConfigManager::loadIniAs(const std::string &file, const std::string &cfgName)
{
    std::ifstream fileStream;
    fileStream.open(file, std::ifstream::in);
    if (!fileStream.is_open()){
        std::cerr << "JHConfigManager::loadIniAs: open file failed," << file << std::endl;
        return false;
    }
    fileStream.close();

    boost::property_tree::ptree lvptProperties;
    boost::property_tree::ini_parser::read_ini(file, lvptProperties);
    auto iter = lvptProperties.begin();
    Json::Value cfgData;
    for (; iter != lvptProperties.end(); ++iter){
        auto iterBasicTree = iter->second.begin();
        Json::Value cfgItem;

        for (;  iterBasicTree != iter->second.end(); iterBasicTree++){
            cfgItem[(*iterBasicTree).first] = (*iterBasicTree).second.data();
        }
        cfgData[iter->first] = cfgItem;
    }
    if (!cfgData.empty()){
        recordData(cfgName,cfgData);
        return true;
    }
    return false;
}

Json::Value XConfigManager::getConfigData(const std::string &cfgName)
{
    auto it = m_configStore.find(cfgName);
    if (it == m_configStore.end()){
        return Json::Value();
    }
    return it->second;
}

std::string XConfigManager::getItemAsString(const std::string &cfgName, const std::string &itemName)
{
    auto it = m_configStore.find(cfgName);
    if (it == m_configStore.end()
            || !it->second.isMember(itemName)){
        return std::string();
    }
    return it->second[itemName].asString();
}

int XConfigManager::getItemAsInt(const std::string &cfgName, const std::string &itemName)
{
    auto it = m_configStore.find(cfgName);
    if (it == m_configStore.end()
            || !it->second.isMember(itemName)){
        return 0;
    }
    return it->second[itemName].asInt();
}

Json::Value XConfigManager::getItemData(const std::string &cfgName, const std::string &itemName)
{
    auto it = m_configStore.find(cfgName);
    if (it == m_configStore.end()
            || !it->second.isMember(itemName)){
        return Json::Value();
    }
    return it->second[itemName];
}

void XConfigManager::addConfigData(const std::string &cfgName, const Json::Value &cfgData)
{
     recordData(cfgName, cfgData);
}

void XConfigManager::recordData(const std::string &cfgName, const Json::Value &cfgData)
{
    auto it =m_configStore.find(cfgName);
    if( it == m_configStore.end()){
        m_configStore.insert(std::make_pair(cfgName, cfgData));
        return;
    }
    for (const auto &key : cfgData.getMemberNames()){
        it->second[key] = cfgData[key];
    }
}
