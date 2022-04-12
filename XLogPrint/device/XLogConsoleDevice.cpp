#include "XLogConsoleDevice.h"
#include <iostream>
XLogConsoleDevice::XLogConsoleDevice()
{

}

XLogConsoleDevice::~XLogConsoleDevice()
{

}

bool XLogConsoleDevice::PrintLog(std::string& log, PriorityLevel level)
{
    if (XLogDeviceBase::PrintLog(log, level) == false)
        return false;

    std::cout << log <<std::endl;
    return true;
}


