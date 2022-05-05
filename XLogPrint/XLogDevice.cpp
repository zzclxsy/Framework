#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "device/XLogConsoleDevice.h"
#include "device/XLogTcpDevice.h"
#include "XLogRule.h"


XLogDevice::XLogDevice(E_DeviceType deviceType)
{
    switch (deviceType)
    {
    case E_DeviceType::CONSOLE:
        mp_currDevice = new XLogConsoleDevice;
        break;
    case E_DeviceType::TCP_DEVICE:
        mp_currDevice = new XLogTcpDevice;
        break;
    default:
        return;
    }
}

XLogDevice::~XLogDevice()
{
    delete mp_rule;
    delete mp_currDevice;
}

void XLogDevice::initialize(Json::Value &data)
{
    mp_currDevice->initialize(data);
}

void XLogDevice::setRule(XLogRule *rule)
{
    mp_rule = rule;
    mp_currDevice->setRule(mp_rule);
}

XLogRule *XLogDevice::getRule()
{
    return mp_rule;
}

XLogDeviceBase *XLogDevice::getDevice()
{
    return mp_currDevice;
}

