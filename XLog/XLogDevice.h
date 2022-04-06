#ifndef XLOGDEVICE_H
#define XLOGDEVICE_H
#include "device/XLogDeviceBase.h"
#include "device/XLogConsoleDevice.h"
#include "XLogRule.h"
class XLogDevice
{
public:
    typedef enum
    {
        CONSOLE
    }E_DeviceType;
    XLogDevice();
    void createDevice(E_DeviceType deviceType);
    void setDeviceRule(XLogRule *rule);

    XLogDeviceBase *currDevice();

private:
    XLogDeviceBase *m_currDevice;
};

#endif // XLOGDEVICE_H
