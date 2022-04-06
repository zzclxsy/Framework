#ifndef XLOGRULE_H
#define XLOGRULE_H
#include <string>

class XLogRule
{
public:
    typedef enum
    {
        EMERG = 0,
        FATAL = 0,
        ALERT = 100,
        CRIT = 200,
        ERROR = 300,
        WARN = 400,
        NOTICE = 500,
        INFO = 600,
        DEBUG = 700,
        NOTSET = 800
    }PriorityLevel;

    XLogRule();
    void setDebugColor(std::string color);
    void setOutputLevel(PriorityLevel level);
    PriorityLevel getOutputLevel();
    std::string startPack(std::string log);

private:
    PriorityLevel m_level;
};

#endif // XLOGRULE_H
