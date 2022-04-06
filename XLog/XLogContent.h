#ifndef XLOGCONTENT_H
#define XLOGCONTENT_H
#include <string>
class XLogDevice;
class XLogDeviceBase;
class XLogContent
{
public:
    XLogContent();
    void setDevive(XLogDevice *device);
    void debug(std::string log);

private:
    XLogDeviceBase *m_currDevice;
};

#endif // XLOGCONTENT_H
