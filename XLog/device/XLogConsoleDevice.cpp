#include "XLogConsoleDevice.h"

XLogConsoleDevice::XLogConsoleDevice()
{

}

bool XLogConsoleDevice::PrintLog(std::string log, XLogRule::PriorityLevel level)
{
    //输出级别过小，不进行输出
    if (mp_rule->getOutputLevel() < level)
        return false;

    return true;
}

void XLogConsoleDevice::debugPrint(std::string log)
{
    std::string pack = mp_rule->startPack(log);


}
