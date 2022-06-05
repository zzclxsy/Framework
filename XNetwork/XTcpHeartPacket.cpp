#include "XTcpHeartPacket.h"
#include "XLogPrint/XLogPrint.h"
#include "XUtils.h"

XTcpHeartPacket::XTcpHeartPacket()
{
    mb_recvHeart = false;

    m_timeroutTimer.SetParam(std::bind(&XTcpHeartPacket::timerOutFunc,this),1000, XTimer::ONCE);
    m_serverTimer.SetParam(std::bind(&XTcpHeartPacket::ServerTimerOutFunc,this),6000,XTimer::ONCE);
    m_sendHeartTimer.SetParam(std::bind(&XTcpHeartPacket::SendHeartFunc,this),3000);
}

void XTcpHeartPacket::SetParameter(closeSocket close, sendCallback send)
{
    m_closeSocket = close;
    m_sendCallback = send;
}

void XTcpHeartPacket::setHeartHander(XTcpHeartPacket::heartHander hander)
{
    m_heartHander = hander;
}

bool XTcpHeartPacket::OnClientRecv(char *data, int length)
{
    Json::Value root;

    if (XUtils::StringToJson(data, length, root) == false)
        return false;

    if (analysisPacket(root))
    {
        mb_recvHeart = true;
        if (m_heartHander)
            m_heartHander("", true);
        return true;
    }
    return false;
}

bool XTcpHeartPacket::OnServerRecv(char *data, int length)
{
    Json::Value root;

    if (XUtils::StringToJson(data, length, root) == false)
        return false;

    if (analysisPacket(root))
    {
        SendPacket();
        m_serverTimer.Stop();
        m_serverTimer.Start();
        if (m_heartHander)
            m_heartHander("rec", true);
        return true;
    }
    return false;
}

void XTcpHeartPacket::Start()
{
    WorkerProc();
}

void XTcpHeartPacket::Stop()
{
    m_sendHeartTimer.Stop();
}

bool XTcpHeartPacket::analysisPacket(Json::Value &root)
{
    if (root.isMember("cmd"))
    {
        if (root["cmd"].asString() == "heart")
        {
            return true;
        }
    }
    return false;
}
void XTcpHeartPacket::SendPacket()
{
    Json::Value root;
    root["cmd"] = "heart";
    std::string Packet = XUtils::JsonToString(root);
    m_sendCallback(Packet.c_str(), Packet.size());
}

void XTcpHeartPacket::WorkerProc()
{
    XDEBUG <<"start heart.";
    m_sendHeartTimer.Start();
}

void XTcpHeartPacket::timerOutFunc()
{
    if (mb_recvHeart)
        return;

    m_sendHeartTimer.Stop();
    m_closeSocket();
    if (m_heartHander)
        m_heartHander("", false);
}

void XTcpHeartPacket::ServerTimerOutFunc()
{
    if (m_heartHander)
        m_heartHander("", false);

}

void XTcpHeartPacket::SendHeartFunc()
{
    SendPacket();
    mb_recvHeart = false;
    m_timeroutTimer.Start();
}
