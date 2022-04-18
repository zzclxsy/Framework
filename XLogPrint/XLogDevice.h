#ifndef XLOGDEVICE_H
#define XLOGDEVICE_H

class XLogRule;
class XLogDeviceBase;
class XLogDevicePrivate;
class XLogDevice
{
public:
    typedef enum
    {
        CONSOLE
    }E_DeviceType;

    XLogDevice(E_DeviceType deviceType);
    ~XLogDevice();

    void setRule(XLogRule *rule);
    XLogRule *getRule();

    XLogDeviceBase *getDevice();

private:
    XLogDevicePrivate *d_ptr;
};

#endif // XLOGDEVICE_H
