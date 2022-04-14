#include "XLogDevice.h"

XLogDevice::XLogDevice(E_DeviceType deviceType)
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


XLogDeviceBase *XLogDevice::device()
{
    return m_currDevice;
}
