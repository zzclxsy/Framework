#ifndef XLOGRULE_H
#define XLOGRULE_H
#include <string>
#include "common.h"
#include <functional>
#include "../global.h"

class XLogRulePrivate;
class FRAMEWORK_EXPORT XLogRule
{
public:

    XLogRule();
    ~XLogRule();
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
    void setCustSave(std::function<void (const std::string& log)> custSave);

    /**
     * @brief 修改级别描述
     */
    void setDebugHeader(const std::string& name);
    const std::string getDebugHeader();

    void setInfoHeader(const std::string& name);
    const std::string getInfoHeader();

    void setWarnHeader(const std::string& name);
    const std::string getWarnHeader();

    void setErrorHeader(const std::string& name);
    const std::string getErrorHeader();

    /**
     * @brief 开始和停止存log
     */
    void stopSave();
    void startSave();

private:
    XLogRulePrivate *d_ptr;
};

#endif // XLOGRULE_H
