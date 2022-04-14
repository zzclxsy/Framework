#include "XLogContent.h"
#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "XLogRule.h"

class XLogContentPrivate
{
public:
    std::vector<XLogDeviceBase *> m_allDevice;
    XLogRule *mp_currRule;

    XLogContent *q_ptr;
};

XLogContent *XLogContent::mp_instant = new XLogContent();
XLogContent::XLogContent()
{
    d_ptr = new XLogContentPrivate;
    d_ptr->q_ptr = this;
    //默认输出到控制台装置
    d_ptr->m_allDevice.push_back(new XLogConsoleDevice);
    mp_instant = this;
    d_ptr->mp_currRule = nullptr;
}

void XLogContent::addDevive(XLogDevice *device)
{
    d_ptr->m_allDevice.push_back(device->device());
    if (d_ptr->mp_currRule != nullptr)
        device->device()->setRule(d_ptr->mp_currRule);
}

void XLogContent::removeDevice(XLogDevice *dev)
{
    auto it = d_ptr->m_allDevice.begin();
    if (it != d_ptr->m_allDevice.end())
    {
        if ((*it) == dev->device())
        {
            d_ptr->m_allDevice.erase(it);
        }
        it++;
    }
}

void XLogContent::setRule(XLogRule *rule)
{
    //所有装置设置相同规则
    d_ptr->mp_currRule = rule;
    for (XLogDeviceBase *dev: d_ptr->m_allDevice)
    {
        dev->setRule(d_ptr->mp_currRule);
    }
}

XLogRule *XLogContent::getRule()
{
    return d_ptr->mp_currRule;
}

void XLogContent::print(PriorityLevel type, std::string log)
{
    //输出到所有装置
    for (XLogDeviceBase *dev: d_ptr->m_allDevice)
    {
        std::string temp = log;
        dev->PrintLog(temp, type);
    }
}

XLogContent *XLogContent::instant()
{
    return mp_instant;
}
