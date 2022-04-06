#include "XLogRule.h"

XLogRule::XLogRule()
{

}

void XLogRule::setDebugColor(std::string color)
{

}

void XLogRule::setOutputLevel(PriorityLevel level)
{
    m_level = level;
}

XLogRule::PriorityLevel XLogRule::getOutputLevel()
{
    return m_level;
}

std::string XLogRule::startPack(std::string log)
{
    return log;
}
