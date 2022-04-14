#ifndef XLOGCONTENT_H
#define XLOGCONTENT_H
#include <string>
#include <sstream>
#include "common.h"
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stdarg.h>
#include "../global.h"
class XLogDevice;
class XLogDeviceBase;
class XLogRule;
class XLogContentPrivate;
class FRAMEWORK_EXPORT XLogContent
{
public:
    XLogContent();
    void addDevive(XLogDevice *device);
    void removeDevice(XLogDevice *device);
    void setRule(XLogRule *rule);
    XLogRule *getRule();
    void print(PriorityLevel type, std::string log);
    static XLogContent *instant();

private:
    static XLogContent *mp_instant;
    XLogContentPrivate *d_ptr;
};

class XLogger
{
public:
    XLogger(PriorityLevel type)
    {
        m_type = type;
    }
    ~XLogger()
    {
        XLogContent::instant()->print(m_type, m_debugLog.str());
    }

    template<typename T>
    XLogger &operator<<(T v)
    {
        m_debugLog << v;
        m_debugLog << " ";
        return (*this);
    }

    template<typename T>
    XLogger& operator << (const std::vector<T>& v)
    {
        m_debugLog << "Vector([";
        auto it = v.begin();
        while (it != v.end())
        {
            m_debugLog << *it;
            if (++it != v.end())
            {
                m_debugLog << ", ";
            }
        }
        m_debugLog << "])";

        return *this;
    }

    template<typename T>
    XLogger& operator << (const std::list<T>& v)
    {

        m_debugLog << "List(";
        auto it = v.begin();
        while (it != v.end())
        {
            m_debugLog << *it;
            if (++it != v.end())
            {
                m_debugLog << "->";
            }
        }
        m_debugLog << ")";

        return *this;
    }

    template<typename T1, typename T2>
    XLogger& operator << (const std::map<T1, T2>& v)
    {

        m_debugLog << "Map(";
        auto it = v.begin();
        while (it != v.end())
        {
            m_debugLog << "(" << it->first << ":" << it->second << ")";

            if (++it != v.end())
            {
                m_debugLog << ", ";
            }
        }
        m_debugLog << ")";

        return *this;
    }

    template<typename T>
    XLogger& operator << (const std::set<T>& v)
    {

        m_debugLog << "Set(";
        auto it = v.begin();
        while (it != v.end())
        {
            m_debugLog << *it;

            if (++it != v.end())
            {
                m_debugLog << ", ";
            }
        }
        m_debugLog << ")";

        return *this;
    }

    XLogger& write(const char *format, ...)
    {
        char buffer[2048] = {0};
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        m_debugLog << buffer;
        return (*this);
    }
private:
    std::stringstream m_debugLog;
    PriorityLevel m_type;
};

#endif // XLOGCONTENT_H
