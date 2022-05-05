#ifndef XLOGDEVICE_H
#define XLOGDEVICE_H
#include "json/json.h"
class XLogRule;
class XLogDeviceBase;
class XLogDevice
{
public:
    typedef enum
    {
        CONSOLE,
        TCP_DEVICE
    }E_DeviceType;

    XLogDevice(E_DeviceType deviceType);
    ~XLogDevice();

    void initialize(Json::Value &data);

    void setRule(XLogRule *rule);
    XLogRule *getRule();

    XLogDeviceBase *getDevice();

private:
    XLogDeviceBase *mp_currDevice;
    XLogRule *mp_rule;
    XLogDevice *q_ptr;
};

#endif // XLOGDEVICE_H
