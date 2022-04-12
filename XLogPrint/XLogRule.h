#ifndef XLOGRULE_H
#define XLOGRULE_H
#include <string>
#include "common.h"
#include <functional>
class XLogRule
{
public:

    XLogRule();
    /**
     * @brief 设置输出级别
     * @param level
     */
    void setOutputLevel(PriorityLevel level);
    PriorityLevel getOutputLevel();

    /**
     * @brief 自定义封装log格式
     * @param printer:函数指针
     */
    void setPrinter(std::function<void (const std::string&)> printer);
    std::string onPacket(const std::string& log);

    /**
     * @brief savePacket,自定义储存文件
     * @param log
     */
    bool savePacket(const std::string& log);
    void setfilePath(const std::string& filePath);
    void setCustSave(std::function<void (const std::string&)> custSave);

    /**
     * @brief 修改级别描述
     */
    void setDebugHeader(const std::string& name){m_debugHeader = name;}
    const std::string getDebugHeader(){return m_debugHeader;}

    void setInfoHeader(const std::string& name){m_infoHeader = name;}
    const std::string getInfoHeader(){return m_infoHeader;}

    void setWarnHeader(const std::string& name){m_warnHeader = name;}
    const std::string getWarnHeader(){return m_warnHeader;}

    void setErrorHeader(const std::string& name){m_errorHeader = name;}
    const std::string getErrorHeader(){return m_errorHeader;}
private:
    PriorityLevel m_level;
    std::function<void (const std::string&)> m_printer;
    std::function<void (const std::string&)> m_custSave;
    std::string m_debugHeader;
    std::string m_warnHeader;
    std::string m_infoHeader;
    std::string m_errorHeader;
    std::string m_filePath;
};

#endif // XLOGRULE_H
