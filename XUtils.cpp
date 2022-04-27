#include "XUtils.h"
#include <thread>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <regex>
#include "XLogPrint/XLogPrint.h"
namespace XUtils{


std::string GetMd5(const char * buffer, int length)
{
    assert(buffer != nullptr);
    boost::uuids::detail::md5 alg;
    alg.process_bytes(buffer, length);
    boost::uuids::detail::md5::digest_type dgst;
    alg.get_digest(dgst);

    std::string result;
    const char * p = reinterpret_cast<const char *>(&dgst);
    boost::algorithm::hex(p, p +sizeof(dgst), std::back_inserter(result));
    return result;
}

std::string GetTextMd5(const std::string& text)
{
    return GetMd5(text.c_str(), (int)text.size());
}

std::string GetFileMd5(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::in);
    if (!file.is_open()){
        return std::string();
    }

    char buffer[8096] = {0};
    int length = 8096;
    boost::uuids::detail::md5 alg;
    while (length){
        file.read(buffer, 8096);
        length = (int)file.gcount();
        alg.process_bytes(buffer, length);
    }
    boost::uuids::detail::md5::digest_type dgst;
    alg.get_digest(dgst);

    std::string result;
    const char * p = reinterpret_cast<const char *>(&dgst);
    boost::algorithm::hex(p, p +sizeof(dgst), std::back_inserter(result));
    return result;
}

std::string StringToLower(const std::string& input)
{
    std::string output;
    boost::to_lower_copy(std::back_inserter(output), input);
    return output;
}

std::string StringToUpper(const std::string& input)
{
    std::string output;
    boost::to_upper_copy(std::back_inserter(output), input);
    return output;
}

std::vector<std::string> StringSplit(const std::string& input, const std::string& spliter)
{
    std::vector<std::string> output;

    size_t pos = 0;
    size_t start = 0;
    do{
        pos = input.find(spliter, start);
        if (pos > start)
        {
            output.push_back(input.substr(start, pos - start));
            start = pos + spliter.size();
        }
    }while(pos != std::string::npos);
    return output;
}

void TrimString(std::string& input)
{
    boost::algorithm::trim_if(input, boost::is_any_of(" \t"));
}

int StringToInt(const std::string& text)
{
    if (text.find("0x") != std::string::npos)
    {
        return strtol(&text.c_str()[2], NULL, 16);
    }
    if (text.find("0X") != std::string::npos)
    {
        return strtol(&text.c_str()[2], NULL, 16);
    }
    return strtol(text.c_str(), NULL, 10);
}

std::string StringFromInt(int number, const char * format)
{
    assert(strlen(format) <= 8);
    char buffer[32] = {0};
    // std::string result(16, '\0');
    sprintf(buffer, format, number);
    return std::string(buffer);
}

double StringToDouble(const std::string& text)
{
    double result;
    std::stringstream ss;
    ss << text;
    ss >> result;
    return result;
}

std::string StringFromDouble(double number)
{
    std::string result;
    std::stringstream ss;
    ss << number;
    ss >> result;
    return result;
}

bool IsInteger(const std::string& text)
{
    for (size_t i = 0; i != text.size(); ++i)
    {
        if (text.at(i) < 0x30 || text.at(i) > 0x39)
        {
            return false;
        }
    }
    return true;
}

bool IsValidIP(const std::string& address)
{
    std::regex pattern("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    return address.empty() ? false : std::regex_match(address.c_str(), pattern);
}

bool MakeDir(const std::string& dirPath)
{
    return boost::filesystem::create_directory(boost::filesystem::path(dirPath));
}

bool MakeDirs(const std::string& dirPath)
{
    return boost::filesystem::create_directories(boost::filesystem::path(dirPath));
}

bool FileExists(const std::string& filePath)
{
    return boost::filesystem::exists(boost::filesystem::path(filePath));
}

std::string GetFileDirName(const std::string& filePath)
{
    size_t npos = filePath.find_last_of("/");
    if (npos > 0)
    {
        return filePath.substr(0, npos);
    }
    npos = filePath.find_last_of("\\");
    if (npos > 0)
    {
        return filePath.substr(0, npos);
    }
    return filePath;
}

bool RemoveFile(const std::string& filePath)
{
    return boost::filesystem::remove(boost::filesystem::path(filePath));
}

bool MoveFile(const std::string& filePath, const std::string& target)
{
    boost::filesystem::path p(target);
    boost::filesystem::rename(boost::filesystem::path(filePath),p);
    return boost::filesystem::exists(p);
}

bool RenameFile(const std::string& filePath, const std::string& target)
{
    boost::filesystem::path p(target);
    boost::filesystem::rename(boost::filesystem::path(filePath), p);
    return boost::filesystem::exists(p);
}

bool ReadTxtFile(const std::string& filePath, std::string& buffer)
{
    std::ifstream fileStream;
    fileStream.open(filePath, std::ifstream::in);
    if (!fileStream.is_open()){
        XERROR << "ReadTxfFile: open file failed," << filePath;
        return false;
    }
    std::stringstream fileContent;
    fileContent << fileStream.rdbuf();
    fileStream.close();

    buffer.clear();
    fileContent >> buffer;
    return true;
}

bool SaveTxtFile(const std::string& filePath, std::string& buffer)
{
    if (!FileExists(filePath))
    {
        MakeDirs(GetFileDirName(filePath));
    }
    std::ofstream fileStream;
    fileStream.open(filePath, std::ofstream::out | std::ofstream ::trunc);
    if (!fileStream.is_open())
    {
        XERROR << "ReadTxfFile: open file failed," << filePath;
        return false;
    }
    fileStream << buffer;
    fileStream.close();

    return true;
}

void MSleep(int millisec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
}

void USleep(int microsec)
{
    std::this_thread::sleep_for(std::chrono::microseconds(microsec));
}

}
