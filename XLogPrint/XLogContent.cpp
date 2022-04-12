#include "XLogContent.h"
#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "XLogRule.h"
XLogContent *XLogContent::mp_instant = new XLogContent();

XLogContent::XLogContent()
{
    m_currDevice = new XLogConsoleDevice;
    mp_instant = this;
}

void XLogContent::setDevive(XLogDevice *device)
{
    m_currDevice = device->currDevice();
}

void XLogContent::setRule(XLogRule *rule)
{
    m_currDevice->setRule(rule);
}

void XLogContent::print(PriorityLevel type, std::string log)
{
    m_currDevice->PrintLog(log, type);
}

XLogContent *XLogContent::instant()
{
    return mp_instant;
}
