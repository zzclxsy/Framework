#ifndef XHTTPHELPER_H
#define XHTTPHELPER_H


#include <string>
#include <map>
#include <functional>

class XHttpHelper
{
public:
    XHttpHelper();
    ~XHttpHelper();

    static std::string syncGet(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        const std::map<std::string, std::string> &data);

    static std::string syncPost(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        const std::map<std::string, std::string> &data);

    static std::string syncDirectPost(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        std::string &data);

    static std::string syncMutilPartPost(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        const std::string &bounary,
        const std::map<std::string, std::string> &data);

    static std::string syncMutilPartDirectPost(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        const std::string &bounary,
        std::string &data);

    static std::string syncJsonDirectPost(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        std::string &data);

    static int asyncGet(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        const std::map<std::string, std::string> &data,
        std::function<void(int, const std::string &)> callback);

    static int asyncPost(
        const std::string &host,
        const std::string &port,
        const std::string &page,
        const std::map<std::string, std::string> &data,
        std::function<void(int, const std::string &)> callback);
};


// const std::string PARAM_SLOT_NAME = "slot";
// const std::string PARAM_STATUS = "status";
// ...

const std::string RESP_KEY_STATUS = "status";
const std::string RESP_KEY_DATA = "data";
const std::string RESP_KEY_ERROR = "error";

const std::string RESP_STATUS_OK = "ok";
const std::string RESP_STATUS_FAILED = "failed";
const std::string RESP_STATUS_ERROR = "error";


#endif // XHTTPHELPER_H
