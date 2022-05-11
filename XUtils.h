#ifndef XUTILS_H
#define XUTILS_H

#include <json/json.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <stdlib.h>

namespace XUtils {


inline void JsonToString(const Json::Value& obj, std::string& output)
{
    Json::StreamWriterBuilder jswBuilder;
    output = Json::writeString(jswBuilder, obj);
}

inline std::string JsonToString(const Json::Value& obj)
{
    Json::StreamWriterBuilder jswBuilder;
    return Json::writeString(jswBuilder, obj);
}

inline bool StringToJson(const std::string& text, Json::Value& output)
{
    try {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        JSONCPP_STRING err;
        return reader->parse(text.c_str(), text.c_str() + text.length(), &output, &err);
    }  catch (...) {
        std::cerr << "StringToJson failed.";
    }
    return false;
}

inline bool StringToJson(const char * data, int length, Json::Value& output)
{
    try {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        JSONCPP_STRING err;
        return reader->parse(data, data+length, &output, &err);
    }  catch (...) {
        std::cerr << "StringToJson failed.";
    }
    return false;
}

inline Json::Value StringToJson(const std::string& text)
{
    Json::Value v;
    try {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        JSONCPP_STRING err;
        bool succ = reader->parse(text.c_str(), text.c_str() + text.length(), &v, &err);
        if (succ){
            return v;
        }
    }  catch (...) {
        std::cerr << "StringToJson failed.";
    }
    return Json::Value();
}

inline Json::Value StringToJson(const char * data, int length)
{
    Json::Value v;
    try {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        JSONCPP_STRING err;
        bool succ = reader->parse(data, data+length, &v, &err);
        if (succ){
            return v;
        }
    }  catch (...) {
        std::cerr << "StringToJson failed.";
    }
    return Json::Value();
}

template <typename T>
inline Json::Value VectorToJson(const std::vector<T>& data)
{
    Json::Value v;
    for (size_t i = 0; i != data.size(); ++i)
    {
        v.append(data.at(i));
    }
    return v;
}

inline std::vector<std::string> JsonToStringList(const Json::Value& data)
{
    std::vector<std::string> v;
    for (unsigned int i = 0; i != data.size(); ++i)
    {
        v.push_back(data[i].asString());
    }
    return v;
}

std::string GetMd5(const char * buffer, int length);
std::string GetTextMd5(const std::string& text);
std::string GetFileMd5(const std::string& filePath);

std::string StringToLower(const std::string& input);
std::string StringToUpper(const std::string& output);

std::vector<std::string> StringSplit(const std::string& input, const std::string& spliter);
void TrimString(std::string& input);

int StringToInt(const std::string& text);
std::string StringFromInt(int number, const char * format);
double StringToDouble(const std::string& text);
std::string StringFromDouble(double number);
bool IsInteger(const std::string& text);

bool IsValidIP(const std::string& address);
bool MakeDir(const std::string& dirPath);
bool MakeDirs(const std::string& dirPath);
bool FileExists(const std::string& filePath);
std::string GetFileDirName(const std::string& filePath);
bool RemoveFile(const std::string& filePath);
bool MoveFile(const std::string& filePath, const std::string& target);
bool RenameFile(const std::string& filePath, const std::string& target);

bool ReadTxtFile(const std::string& filePath, std::string& buffer);

bool SaveTxtFile(const std::string& filePath, std::string& buffer);

void MSleep(int millisec);
void USleep(int microsec);

}
#endif // XUTILS_H
