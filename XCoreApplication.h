#ifndef XCOREAPPLICATION_H
#define XCOREAPPLICATION_H
#include <vector>
#include <string>
class XEvent;
class XEventLoop;
class XCoreApplication
{
public:
    XCoreApplication(int argc, char **argv);
    void exec();
    void quit();
    static XCoreApplication *instant();

private:
    void addEvent(XEvent *event);
    void removeEvent(XEvent *event);

private:
    std::vector<XEvent *> m_allEvent;
    bool m_isQuit;
    std::string m_filePath;
    std::string m_fileName;
    std::vector<std::string> m_allParam;
    friend XEvent;
    friend XEventLoop;
};

#endif // XCOREAPPLICATION_H
