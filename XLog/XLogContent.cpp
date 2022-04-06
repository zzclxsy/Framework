#include "XLogContent.h"
#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "XLogRule.h"
XLogContent::XLogContent()
{

}

void XLogContent::setDevive(XLogDevice *device)
{
    m_currDevice = device->currDevice();
}

void XLogContent::debug(std::string log)
{
    m_currDevice->PrintLog(log, XLogRule::DEBUG);
}
