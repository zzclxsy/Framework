#include "XLogTcpDevice.h"
#include "XLogPrint/XLogPrint.h"
#include "XCoreApplication.h"
#include "XApi/VXApi.h"
#include "memory"

XLogTcpDevice::XLogTcpDevice()
{

}

bool XLogTcpDevice::PrintLog(std::string &log, logLevel level)
{
    if (m_logClient.use_count() == 0)
        return false;

    if (XLogDeviceBase::PrintLog(log, level) == false)
        return false;

    if (level == logLevel::E_DEBUG)
        log = "debugLog$" + log;
    else if (level == logLevel::E_ERROR)
        log = "errorLog$" + log;
    else if (level == logLevel::E_INFO)
        log = "infoLog$" + log;
    else if (level == logLevel::E_WARN)
        log = "warnLog$" + log;

    if (m_logClient->SendDataAsync(log.c_str(),log.size()) == -1)
    {
        XERROR <<"log send fail";
        return false;
    }
    return true;
}

bool XLogTcpDevice::initialize(const Json::Value &data)
{
    std::string ip;
    if (data.isMember("logServerIP"))
    {
        m_serverIP  = data["logServerIP"].asString();
    }
    else
    {
        XERROR << "XLogTcpDevice::initialize: no find logServerIP";
        return false;
    }

    if (data.isMember("logServerPort"))
    {
        m_serverPort  = data["logServerPort"].asInt();
    }
    else
    {
        XERROR << "XLogTcpDevice::initialize: no find logServerPort";
        return false;
    }

    m_logClient = XCoreApplication::GetApp()->CreateTcpClient();
    m_logClient->SetServerIp(m_serverIP);
    m_logClient->SetServerPort(m_serverPort);
    //m_logClient->SetHeartCheck();
    m_logClient->Start();

    return true;
}
