#include "XLogDevice.h"

XLogDevice::XLogDevice()
{
    m_currDevice = nullptr;
}

void XLogDevice::createDevice(E_DeviceType deviceType)
{
    switch (deviceType)
    {
    case E_DeviceType::CONSOLE:
        m_currDevice = new XLogConsoleDevice;
        break;
    default:
        break;
    }
}

void XLogDevice::setDeviceRule(XLogRule *rule)
{
    if (m_currDevice)
        return;

    m_currDevice->setRule(rule);
}

XLogDeviceBase *XLogDevice::currDevice()
{
    return m_currDevice;
}
