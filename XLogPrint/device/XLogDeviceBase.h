#ifndef XLOGDEVICEBASE_H
#define XLOGDEVICEBASE_H
#include <string>
#include "../XLogRule.h"

class XLogDeviceBase
{
public:
    XLogDeviceBase()
    {
        mp_rule = nullptr;
    }
    virtual ~XLogDeviceBase(){};
    void setRule(XLogRule *rule){mp_rule = rule;}

    //基类做最基本的数据处理
    virtual bool PrintLog(std::string& log, PriorityLevel level)
    {
        std::string header;
        switch (level)
        {
        case PriorityLevel::DEBUG:
            header = "<DEBUG> ";
            break;

        case PriorityLevel::INFO:
            header = "<INFO> ";
            break;

        case PriorityLevel::WARN:
            header = "<WARN> ";
            break;

        case PriorityLevel::ERROR:
            header = "<ERROR> ";
            break;
        default:
            break;
        }

        if (mp_rule == nullptr)
        {
            log = header + log;
            return true;
        }

        if (level >mp_rule->getOutputLevel())
            return false;

        std::string pack = mp_rule->onPacket(log);
        log = header + pack;

        return true;
    }

protected:
    XLogRule *mp_rule;
};

#endif // XLOGDEVICEBASE_H
