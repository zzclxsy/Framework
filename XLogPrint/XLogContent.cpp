#include "XLogContent.h"
#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "XLogRule.h"
XLogContent *XLogContent::mp_instant = new XLogContent();

XLogContent::XLogContent()
{
    //默认输出到控制台装置
    m_allDevice.push_back(new XLogConsoleDevice);
    mp_instant = this;
    mp_currRule = nullptr;
}

void XLogContent::addDevive(XLogDevice *device)
{
    m_allDevice.push_back(device->currDevice());
    if (mp_currRule != nullptr)
        device->currDevice()->setRule(mp_currRule);
}

void XLogContent::removeDevice(XLogDevice *device)
{
    auto it = m_allDevice.begin();
    if (it != m_allDevice.end())
    {
        if ((*it) == device->currDevice())
        {
            m_allDevice.erase(it);
        }
        it++;
    }
}

void XLogContent::setRule(XLogRule *rule)
{
    //所有装置设置相同规则
    mp_currRule = rule;
    for (XLogDeviceBase *dev: m_allDevice)
    {
        dev->setRule(mp_currRule);
    }
}

void XLogContent::print(PriorityLevel type, std::string log)
{
    //输出到所有装置
    for (XLogDeviceBase *dev: m_allDevice)
    {
        std::string temp = log;
        dev->PrintLog(temp, type);
    }
}

XLogContent *XLogContent::instant()
{
    return mp_instant;
}
