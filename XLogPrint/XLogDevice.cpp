#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "device/XLogConsoleDevice.h"
#include "XLogRule.h"

class XLogDevicePrivate
{
public:
    ~XLogDevicePrivate()
    {
        delete mp_rule;
        delete mp_currDevice;
    }
    XLogDeviceBase *mp_currDevice;
    XLogRule *mp_rule;
    XLogDevice *q_ptr;
};

XLogDevice::XLogDevice(E_DeviceType deviceType)
{
    d_ptr = new XLogDevicePrivate;
    d_ptr->q_ptr = this;

    switch (deviceType)
    {
    case E_DeviceType::CONSOLE:
        d_ptr->mp_currDevice = new XLogConsoleDevice;
        break;
    default:
        return;
    }
}

XLogDevice::~XLogDevice()
{
    delete d_ptr;
}

void XLogDevice::setRule(XLogRule *rule)
{
    d_ptr->mp_rule = rule;
    d_ptr->mp_currDevice->setRule(d_ptr->mp_rule);
}

XLogRule *XLogDevice::getRule()
{
    return d_ptr->mp_rule;
}

XLogDeviceBase *XLogDevice::getDevice()
{
    return d_ptr->mp_currDevice;
}

