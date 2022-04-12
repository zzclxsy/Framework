#include "XLogRule.h"
#include "XTime/XTime.h"

XLogRule::XLogRule()
{
    m_printer = nullptr;
    m_custSave = nullptr;
    m_filePath = "";
    m_debugHeader = "<debug> ";
    m_warnHeader  = "<warn> ";
    m_infoHeader  = "<info> ";
    m_errorHeader = "<error> ";
}

void XLogRule::setOutputLevel(PriorityLevel level)
{
    m_level = level;
}

PriorityLevel XLogRule::getOutputLevel()
{
    return m_level;
}

void XLogRule::setPrinter(std::function<void (const std::string &)> printer)
{
    m_printer = printer;
}

std::string XLogRule::onPacket(const std::string &log)
{
    std::string pack;
    if (m_printer != nullptr)
    {
        m_printer(log);
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
    if (m_custSave == nullptr)
    {
        if (m_filePath == "")
            return false;


        return true;
    }
    m_custSave(m_filePath);
    return true;
}

void XLogRule::setfilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

void XLogRule::setCustSave(std::function<void (const std::string &)> custSave)
{
    m_custSave = custSave;
}
