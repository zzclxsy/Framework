#ifndef XLOGDEVICE_H
#define XLOGDEVICE_H
#include "device/XLogDeviceBase.h"
#include "device/XLogConsoleDevice.h"
#include "XLogRule.h"
#include "../global.h"
class FRAMEWORK_EXPORT XLogDevice
{
public:
    typedef enum
    {
        CONSOLE
    }E_DeviceType;

    XLogDevice(E_DeviceType deviceType);

    XLogDeviceBase *device();

private:
    XLogDeviceBase *m_currDevice;
};

#endif // XLOGDEVICE_H
