#ifndef XCOREAPPLICATION_H
#define XCOREAPPLICATION_H
#include <vector>
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

    void initTimeModule();

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
