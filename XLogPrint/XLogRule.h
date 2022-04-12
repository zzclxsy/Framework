#ifndef XLOGRULE_H
#define XLOGRULE_H
#include <string>
#include "common.h"
#include <functional>
class XLogRule
{
public:

    XLogRule();
    void setOutputLevel(PriorityLevel level);
    PriorityLevel getOutputLevel();

    /**
     * @brief 自定义封装log格式
     * @param printer:函数指针
     */
    void setPrinter(std::function<void (const std::string&)> printer);
    std::string onPacket(const std::string& log);

    void savePacket(std::string filePath);

private:
    PriorityLevel m_level;
    std::function<void (const std::string&)> m_printer;
};

#endif // XLOGRULE_H
