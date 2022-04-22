#include "XTcpClient.h"

//typedef boost::asio::ip::tcp TCP;
//class XTcpClientPrivate
//{
//public:
//    std::thread *m_worker;
//    bool m_running;

//    TCP::socket * m_sk;
//    TCP::endpoint m_endPoint;

//    XTcpClient *q_ptr;
//};

XTcpClient::XTcpClient()
{
//    d_ptr = new XTcpClientPrivate;
//    d_ptr->q_ptr = this;

//    d_ptr->m_worker = nullptr;
//    d_ptr->m_running = false;
//    d_ptr->m_sk= nullptr;
}

bool XTcpClient::Start()
{
//    if (d_ptr->m_worker != nullptr)
//    {
//        return d_ptr->m_running;
//    }

//    TCP::endpoint localEndPoint(boost::asio::ip::tcp::v4(), 0);
//    d_ptr->m_sk = new TCP::socket(m_ioctx);

//    d_ptr->m_running = true;
//    d_ptr->m_worker = new std::thread([this](){
//        try{
//            this->WorkerProc();
//        }
//        catch(std::exception e)
//        {
//           // XERROR << "std::exception from JHTcpClient.WorkerProc catched:" << e.what();
//        }
//        catch(...)
//        {
//            //XERROR << "unhandled exception from JHTcpClient.WorkerProc catched.";
//        }
//    });

    return true;
}

void XTcpClient::WorkerProc()
{

}
