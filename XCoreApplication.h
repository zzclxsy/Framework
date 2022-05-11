#ifndef XCOREAPPLICATION_H
#define XCOREAPPLICATION_H
#include <vector>
#include <string>
#include <memory>
class VXTcpServer;
class VXTcpClient;
class VXFactory;
class VXModule;
class XEvent;
class XEventLoop;
class XCoreApplicationPrivate;
class  XCoreApplication
{
public:
    XCoreApplication(int argc, char **argv);
    void exec();
    void quit();

    std::shared_ptr<VXTcpClient> CreateTcpClient();
    std::shared_ptr<VXTcpServer> CreateTcpServer();

    static XCoreApplication *GetApp();

    int Register(VXFactory * factory);
    VXModule *GetModule(int id);

    template <class T>
    static inline T * GetXService(int id)
    {
        XCoreApplication * app = XCoreApplication::GetApp();
        if (!app){
            return nullptr;
        }
        return dynamic_cast<T *>(app->GetModule(id));
    }

    std::string GetAppPath();
    std::string GetAppName();
    std::vector<std::string> GetArguments();

private:
    void addEvent(XEvent *event);
    void removeEvent(XEvent *event);
    std::vector<XEvent *> getAllEvent();

private:
    friend XEvent;
    friend XEventLoop;
    XCoreApplicationPrivate *d_ptr;
};

#endif // XCOREAPPLICATION_H
