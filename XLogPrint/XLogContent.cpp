#include "XLogContent.h"
#include "XLogDevice.h"
#include "device/XLogDeviceBase.h"
#include "XLogRule.h"

#define ADD_DEVICE(NAME, CLASS) std::pair<std::string, XLogDevice *>(NAME, CLASS)
class XLogContentPrivate
{
public:

};

XLogContent *XLogContent::mp_instant = new XLogContent();
XLogContent::XLogContent()
{
    //默认输出到控制台装置
    XLogDevice *device =new XLogDevice(XLogDevice::CONSOLE);
    device->setRule(new XLogRule);

    m_allDevice.insert(ADD_DEVICE("console", device));
    mp_instant = this;
}

XLogContent::~XLogContent()
{

}

void XLogContent::addDevive(XLogDevice *device, std::string alias)
{
    m_mutex.lock();
    m_allDevice.insert(ADD_DEVICE(alias, device));
    m_mutex.unlock();
}

void XLogContent::removeDevice(std::string alias)
{
    auto it = m_allDevice.begin();
    while (it != m_allDevice.end())
    {
        if ((*it).first == alias)
        {
            XLogDevice *desDev = (*it).second;
            m_allDevice.erase(it);
            delete desDev;
        }
        it++;
    }
}

XLogDevice *XLogContent::getDevice(std::string alias)
{
    m_mutex.lock();
    auto it = m_allDevice.begin();
    while (it != m_allDevice.end())
    {
        if ((*it).first == alias)
            return ((*it).second);
        it++;
    }
    m_mutex.unlock();
    return nullptr;
}


void XLogContent::print(logLevel type, std::string log, std::string &devName)
{
    //指定装置
    if (devName != "")
    {
        XLogDevice * dev = getDevice(devName);
        if (dev !=nullptr)
            dev->getDevice()->PrintLog(log, type);

        return;
    }

    //非指定装置，输出默认控制台
    XLogDevice * dev = getDevice("console");
    if (dev !=nullptr)
        dev->getDevice()->PrintLog(log, type);
}

XLogContent *XLogContent::instant()
{
    return mp_instant;
}
