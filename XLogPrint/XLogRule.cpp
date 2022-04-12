#include "XLogRule.h"
#include "XTime/XTime.h"
XLogRule::XLogRule()
{
    m_printer = nullptr;
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
