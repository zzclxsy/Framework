#include "XLogRule.h"
#include "XApi/VXApi.h"
#include "boost/filesystem.hpp"
#include <mutex>
#include <map>
#include "iostream"
#include <fstream>
class XLogRulePrivate
{
    typedef  std::function<void (std::string&)> callback;

public:
    void addCallback(callback call, std::string& alias)
    {
        m_allCallback.insert(std::pair<std::string, callback>(alias, call));
    }
    callback findCallback(std::string& alias)
    {
        for (std::pair<std::string, callback> it : m_allCallback)
        {
            if (it.first == alias)
                return it.second;
        }
        return nullptr;
    }
public:
    logLevel m_level;
    callback m_printer;
    callback m_custSave;
    std::string m_debugHeader;
    std::string m_warnHeader;
    std::string m_infoHeader;
    std::string m_errorHeader;
    std::string m_filePath;
    std::string m_fileName;
    std::fstream m_file;
    bool mb_saveFlag;
    std::mutex m_mutex;
    std::map<std::string, callback> m_allCallback;
    XLogRule *q_ptr;
};

XLogRule::XLogRule()
{
    d_ptr = new XLogRulePrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_printer = nullptr;
    d_ptr->m_custSave = nullptr;
    d_ptr->m_level = logLevel::E_DEBUG;
    d_ptr->m_filePath = "";
    d_ptr->m_debugHeader = "[debug] ";
    d_ptr->m_warnHeader  = "[warn] ";
    d_ptr->m_infoHeader  = "[info] ";
    d_ptr->m_errorHeader = "[error] ";
    d_ptr->mb_saveFlag = false;
    d_ptr->m_fileName = "logFile.ini";
}

XLogRule::~XLogRule()
{
    delete d_ptr;
}

void XLogRule::setOutputLevel(logLevel level)
{
    d_ptr->m_level = level;
}

logLevel XLogRule::getOutputLevel()
{
    return d_ptr->m_level;
}

void XLogRule::addPrinterFunc(std::function<void (std::string &)> printer, std::string alias)
{
    d_ptr->m_printer = printer;
    d_ptr->addCallback(printer, alias);
}

std::string XLogRule::onPacket(std::string &log)
{
    std::string pack;
    if (d_ptr->m_printer != nullptr)
    {
        d_ptr->m_printer(log);
        pack = log;
    }
    else
    {
        pack = "[" + XGetTimeModule()->toTimeString() + "] " + log;
    }

    return pack;
}

void XLogRule::selectPacketFunc(std::string &alias)
{
    d_ptr->m_printer = d_ptr->findCallback(alias);
}

bool XLogRule::savePacket(std::string& log)
{
    if (d_ptr->mb_saveFlag == false)
        return false;

    if (d_ptr->m_custSave == nullptr)
    {
        if (d_ptr->m_filePath == "")
            return false;;

        d_ptr->m_mutex.lock();
        d_ptr->m_file << log << "\n";
        d_ptr->m_file.flush();
        d_ptr->m_mutex.unlock();

        return true;
    }
    d_ptr->m_custSave(log);
    return true;
}

void XLogRule::setFileName(std::string fileName)
{
    std::string filePath;
    std::string name;
    for (unsigned short i = 0; i < fileName.size(); i++)
    {
        if (fileName[i] == '\\')
            fileName[i] = '/';
    }
    if (fileName.find_last_of('/') == std::string::npos)
    {
        filePath = "./";
        name = fileName;
    }
    else
    {
        filePath = fileName.substr(0, fileName.find_last_of('/'));
        name = fileName.substr(fileName.find_last_of('/') + 1);
    }
    if (boost::filesystem::exists(filePath) == false)
        boost::filesystem::create_directories(filePath);

    d_ptr->m_fileName = name;
    d_ptr->m_filePath = filePath;
    startSave();
}

void XLogRule::addCustomSaveFunc(std::function<void (std::string &)> custSave, std::string alias)
{
    d_ptr->m_custSave = custSave;
    d_ptr->addCallback(custSave, alias);
}

void XLogRule::selectCustomSaveFunc(std::string &alias)
{
    d_ptr->m_custSave = d_ptr->findCallback(alias);
}

void XLogRule::setDebugHeader(const std::string &name)
{
    d_ptr->m_debugHeader = name;
}

const std::string XLogRule::getDebugHeader()
{
    return d_ptr->m_debugHeader;
}

void XLogRule::setInfoHeader(const std::string &name)
{
    d_ptr->m_infoHeader = name;
}

const std::string XLogRule::getInfoHeader()
{
    return d_ptr->m_infoHeader;
}

void XLogRule::setWarnHeader(const std::string &name)
{
    d_ptr->m_warnHeader = name;
}

const std::string XLogRule::getWarnHeader()
{
    return d_ptr->m_warnHeader;
}

void XLogRule::setErrorHeader(const std::string &name)
{
    d_ptr->m_errorHeader = name;
}

const std::string XLogRule::getErrorHeader()
{
    return d_ptr->m_errorHeader;
}

void XLogRule::stopSave()
{
    d_ptr->mb_saveFlag = false;
    d_ptr->m_file.close();
}

void XLogRule::startSave()
{
    d_ptr->mb_saveFlag = true;
    d_ptr->m_file.open(d_ptr->m_filePath +"/"+ d_ptr->m_fileName, std::ios::app);
}
