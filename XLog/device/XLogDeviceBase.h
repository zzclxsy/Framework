#ifndef XLOGDEVICEBASE_H
#define XLOGDEVICEBASE_H
#include <string>
#include "XLog/XLogRule.h"

class XLogDeviceBase
{
public:
    void setRule(XLogRule *rule){mp_rule = rule;}
    virtual bool PrintLog(std::string log, XLogRule::PriorityLevel level);

protected:
    XLogRule *mp_rule;
};

#endif // XLOGDEVICEBASE_H
