#ifndef XTCPHEARTPACKET_H
#define XTCPHEARTPACKET_H
#include <functional>
#include <string>
#include "XTimer/XTimer.h"
#include <memory>
#include "json/json.h"
class XTcpHeartPacket
{
    typedef std::function<int (const char * data, int length)> sendCallback;
    typedef std::function<void ()> closeSocket;
    typedef std::function<void (std::string, bool)> heartHander;
public:
    XTcpHeartPacket();
    void SetParameter(closeSocket close, sendCallback send);
    void setHeartHander(heartHander hander);

    bool OnClientRecv(char * data, int length);
    bool OnServerRecv(char * data, int length);
    void Start();
    void Stop();

    bool analysisPacket(Json::Value&);
    void SendPacket();

private:
    void WorkerProc();
    void timerOutFunc();
    void ServerTimerOutFunc();
    void SendHeartFunc();
private:
    sendCallback m_sendCallback;
    closeSocket m_closeSocket;
    heartHander m_heartHander;
    bool mb_recvHeart;
    XTimer m_sendHeartTimer;
    XTimer m_timeroutTimer;
    XTimer m_serverTimer;
};

#endif // XTCPHEARTPACKET_H
