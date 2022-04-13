#include "XLogRule.h"
#include "XTime/XTime.h"

class XLogRulePrivate
{
public:
    PriorityLevel m_level;
    std::function<void (const std::string&)> m_printer;
    std::function<void (const std::string&)> m_custSave;
    std::string m_debugHeader;
    std::string m_warnHeader;
    std::string m_infoHeader;
    std::string m_errorHeader;
    std::string m_filePath;

    XLogRule *q_ptr;
};
XLogRule::XLogRule()
{
    d_ptr = new XLogRulePrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_printer = nullptr;
    d_ptr->m_custSave = nullptr;
    d_ptr->m_filePath = "";
    d_ptr->m_debugHeader = "<debug> ";
    d_ptr->m_warnHeader  = "<warn> ";
    d_ptr->m_infoHeader  = "<info> ";
    d_ptr->m_errorHeader = "<error> ";
}

void XLogRule::setOutputLevel(PriorityLevel level)
{
    d_ptr->m_level = level;
}

PriorityLevel XLogRule::getOutputLevel()
{
    return d_ptr->m_level;
}

void XLogRule::setPrinter(std::function<void (const std::string &)> printer)
{
    d_ptr->m_printer = printer;
}

std::string XLogRule::onPacket(const std::string &log)
{
    std::string pack;
    if (d_ptr->m_printer != nullptr)
    {
        d_ptr->m_printer(log);
        pack = log;
    }
    else
    {
        pack = "[" + XTime::instant()->toTimeString() + "]" + log;
    }

    return pack;
}

bool XLogRule::savePacket(const std::string& log)
{
    if (d_ptr->m_custSave == nullptr)
    {
        if (d_ptr->m_filePath == "")
            return false;


        return true;
    }
    d_ptr->m_custSave(d_ptr->m_filePath);
    return true;
}

void XLogRule::setfilePath(const std::string &filePath)
{
    d_ptr->m_filePath = filePath;
}

void XLogRule::setCustSave(std::function<void (const std::string &)> custSave)
{
    d_ptr->m_custSave = custSave;
}

void XLogRule::setDebugHeader(const std::string &name)
{
    d_ptr->m_debugHeader = name;
}

const std::string XLogRule::getDebugHeader()
{
    return d_ptr->m_debugHeader;
}

void XLogRule::setInfoHeader(const std::string &name)
{
    d_ptr->m_infoHeader = name;
}

const std::string XLogRule::getInfoHeader()
{
    return d_ptr->m_infoHeader;
}

void XLogRule::setWarnHeader(const std::string &name)
{
    d_ptr->m_warnHeader = name;
}

const std::string XLogRule::getWarnHeader()
{
    return d_ptr->m_warnHeader;
}

void XLogRule::setErrorHeader(const std::string &name)
{
    d_ptr->m_errorHeader = name;
}

const std::string XLogRule::getErrorHeader()
{
    return d_ptr->m_errorHeader;
}
