#include "XLogContent.h"
#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "XLogRule.h"
#include <mutex>
#define ADD_DEVICE(NAME, CLASS) std::pair<std::string, XLogDevice *>(NAME, CLASS)
class XLogContentPrivate
{
public:
    std::map<std::string, XLogDevice *> m_allDevice;
    std::mutex m_mutex;
    XLogContent *q_ptr;
};

XLogContent *XLogContent::mp_instant = new XLogContent();
XLogContent::XLogContent()
{
    d_ptr = new XLogContentPrivate;
    d_ptr->q_ptr = this;

    //默认输出到控制台装置
    XLogDevice *device =new XLogDevice(XLogDevice::CONSOLE);
    device->setRule(new XLogRule);

    d_ptr->m_allDevice.insert(ADD_DEVICE("console", device));
    mp_instant = this;
}

XLogContent::~XLogContent()
{
    delete d_ptr;
}

void XLogContent::addDevive(XLogDevice *device, std::string alias)
{
    d_ptr->m_allDevice.insert(ADD_DEVICE(alias, device));
}

void XLogContent::removeDevice(std::string alias)
{
    d_ptr->m_mutex.lock();
    auto it = d_ptr->m_allDevice.begin();
    while (it != d_ptr->m_allDevice.end())
    {
        if ((*it).first == alias)
        {
            XLogDevice *desDev = (*it).second;
            d_ptr->m_allDevice.erase(it);
            delete desDev;
        }
        it++;
    }
    d_ptr->m_mutex.unlock();
}

XLogDevice *XLogContent::getDevice(std::string alias)
{
    auto it = d_ptr->m_allDevice.begin();
    while (it != d_ptr->m_allDevice.end())
    {
        if ((*it).first == alias)
            return ((*it).second);
        it++;
    }

    return nullptr;
}


void XLogContent::print(PriorityLevel type, std::string log, std::string &devName)
{
    //指定装置
    if (devName != "")
    {
        XLogDevice * dev = getDevice(devName);
        if (dev !=nullptr)
            dev->getDevice()->PrintLog(log, type);
        return;
    }

    //非指定装置，输出到所有装置
    d_ptr->m_mutex.lock();
    auto it = d_ptr->m_allDevice.begin();
    while (it != d_ptr->m_allDevice.end())
    {
        std::string temp = log;
        (*it).second->getDevice()->PrintLog(temp, type);
        it++;
    }
    d_ptr->m_mutex.unlock();
}

XLogContent *XLogContent::instant()
{
    return mp_instant;
}
