#include "XHttpHelper.h"
#include "XHttpKeys.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace asio = boost::asio;     // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

XHttpHelper::XHttpHelper()
{
}

XHttpHelper::~XHttpHelper()
{
}

std::string XHttpHelper::syncGet(
    const std::string &host,
    const std::string &port,
    const std::string &page,
    const std::map<std::string, std::string> &data)
{
    try
    {
        asio::io_context ioCtx;
        asio::ip::tcp::resolver resolver(ioCtx);
        // beast::tcp_stream tcpStream(ioCtx);
        tcp::socket tcpStream{ioCtx};

        auto endPoint = resolver.resolve(host, port);
        // tcpStream.connect(endPoint);
        asio::connect(tcpStream, endPoint.begin(), endPoint.end());

        std::string httpQuery = page + "?";
        std::map<std::string, std::string>::const_iterator it = data.begin();
        while (it != data.end())
        {
            httpQuery += it->first + "=" + it->second;
            ++it;
            if (it != data.end())
            {
                httpQuery += "&";
            }
        }

        http::request<http::string_body> req{http::verb::get, httpQuery, 10};
        // req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(tcpStream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(tcpStream, buffer, res);

        beast::error_code rc;
        // tcpStream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, rc);
        tcpStream.shutdown(tcp::socket::shutdown_both, rc);

        if (rc && rc != beast::errc::not_connected)
        {
            throw beast::system_error{rc};
        }

        return res.body();
    }
    catch (std::exception const &e)
    {
        std::cerr << "XHttpHelper::get, error: " << e.what() << std::endl;
        std::cerr << "http host : " << host << " Port : " << port << std::endl;
        return "";
    }
}

std::string XHttpHelper::syncPost(
    const std::string &host,
    const std::string &port,
    const std::string &page,
    const std::map<std::string, std::string> &data)
{
    try
    {

        asio::io_context ioCtx;
        asio::ip::tcp::resolver resolver(ioCtx);
        // beast::SyncWriteStream tcpStream(ioCtx);
        tcp::socket tcpStream{ioCtx};

        auto endPoint = resolver.resolve(host, port);
        // tcpStream.connect(endPoint);
        asio::connect(tcpStream, endPoint.begin(), endPoint.end());

        http::request<http::string_body> req{http::verb::post, page, 10};
        // req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // std::stringstream bodyData;
        std::map<std::string, std::string>::const_iterator it = data.begin();
        std::string &body = req.body();
        while (it != data.end())
        {
            body += it->first;
            body += "=";
            body += it->second;
            // bodyData << it->first << "=" << it->second;
            ++it;
            if (it != data.end())
            {
                // bodyData << "&";
                body += "&";
            }
        }
        body += HTTP_DATA_PADDING;
        // bodyData >> req.body();
        // std::cout << "req body data size:" << req.body().size() << std::endl;

        req.set(http::field::content_type, "application/x-www-form-urlencoded;charset=utf-8");
        req.set(http::field::content_length, std::to_string(req.body().size()));

        http::write(tcpStream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(tcpStream, buffer, res);

        beast::error_code rc;
        // tcpStream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, rc);
        tcpStream.shutdown(tcp::socket::shutdown_both, rc);

        if (rc && rc != beast::errc::not_connected)
        {
            throw beast::system_error{rc};
        }

        return res.body();
    }
    catch (std::exception const &e)
    {
        std::cerr << "XHttpHelper::post, error: " << e.what() << std::endl;
        std::cerr << "http host : " << host << " Port : " << port << std::endl;
        return std::string();
    }
    return std::string();
}
std::string XHttpHelper::syncDirectPost(
    const std::string &host,
    const std::string &port,
    const std::string &page,
    std::string &data)
{
    try
    {

        asio::io_context ioCtx;
        asio::ip::tcp::resolver resolver(ioCtx);
        // beast::SyncWriteStream tcpStream(ioCtx);
        tcp::socket tcpStream{ioCtx};

        auto endPoint = resolver.resolve(host, port);
        // tcpStream.connect(endPoint);
        asio::connect(tcpStream, endPoint.begin(), endPoint.end());

        http::request<http::string_body> req{http::verb::post, page, 10};
        // req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // std::stringstream bodyData;
        std::string &body = req.body();
        body.swap(data);
        // bodyData >> req.body();
        // std::cout << "req body data size:" << req.body().size() << std::endl;

        req.set(http::field::content_type, "application/x-www-form-urlencoded;charset=utf-8");
        req.set(http::field::content_length, std::to_string(req.body().size()));

        http::write(tcpStream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(tcpStream, buffer, res);
        // swap body data
        data.swap(body);
        //
        beast::error_code rc;
        // tcpStream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, rc);
        tcpStream.shutdown(tcp::socket::shutdown_both, rc);

        if (rc && rc != beast::errc::not_connected)
        {
            throw beast::system_error{rc};
        }

        return res.body();
    }
    catch (std::exception const &e)
    {
        std::cerr << "XHttpHelper::post, error: " << e.what() << std::endl;
        std::cerr << "http host : " << host << " Port : " << port << std::endl;
        return std::string();
    }
    return std::string();
}
std::string XHttpHelper::syncMutilPartPost(
    const std::string &host,
    const std::string &port,
    const std::string &page,
    const std::string &bounary,
    const std::map<std::string, std::string> &data)
{
    try
    {
        asio::io_context ioCtx;
        asio::ip::tcp::resolver resolver(ioCtx);
        // beast::SyncWriteStream tcpStream(ioCtx);
        tcp::socket tcpStream{ioCtx};

        auto endPoint = resolver.resolve(host, port);
        // tcpStream.connect(endPoint);
        asio::connect(tcpStream, endPoint.begin(), endPoint.end());

        http::request<http::string_body> req{http::verb::post, page, 10};
        // req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // std::stringstream bodyData;
        std::map<std::string, std::string>::const_iterator it = data.begin();
        std::string &body = req.body();
        std::string partBounary = "--" + bounary + "\r\n";
        std::string endBounary = "--" + bounary + "--" + "\r\n";
        while (it != data.end())
        {
            body += partBounary;
            body += "Content-Disposition:form-data;name=\"" + it->first + "\"\r\n\r\n";
            body += it->second;
            body += "\r\n";
            // bodyData << it->first << "=" << it->second;
            ++it;
            if (it == data.end())
            {
                body += endBounary;
            }
        }
        // req.set(http::field::content_type, "application/x-www-form-urlencoded;charset=utf-8");
        req.set(http::field::content_type, "multipart/form-data;charset=utf-8;boundary=" + bounary);
        req.set(http::field::content_length, std::to_string(req.body().size()));

        http::write(tcpStream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(tcpStream, buffer, res);

        beast::error_code rc;
        // tcpStream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, rc);
        tcpStream.shutdown(tcp::socket::shutdown_both, rc);

        if (rc && rc != beast::errc::not_connected)
        {
            throw beast::system_error{rc};
        }

        return res.body();
    }
    catch (std::exception const &e)
    {
        std::cerr << "XHttpHelper::post, error: " << e.what() << std::endl;
        std::cerr << "http host : " << host << " Port : " << port << std::endl;
        return std::string();
    }
    return std::string();
}
std::string XHttpHelper::syncMutilPartDirectPost(
    const std::string &host,
    const std::string &port,
    const std::string &page,
    const std::string &bounary,
    std::string &data)
{
    try
    {

        asio::io_context ioCtx;
        asio::ip::tcp::resolver resolver(ioCtx);
        tcp::socket tcpStream{ioCtx};

        auto endPoint = resolver.resolve(host, port);
        asio::connect(tcpStream, endPoint.begin(), endPoint.end());

        http::request<http::string_body> req{http::verb::post, page, 10};
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        std::string &body = req.body();
        body.swap(data);
        // req.set(http::field::content_type, "application/x-www-form-urlencoded;charset=utf-8");
        req.set(http::field::content_type, "multipart/form-data;charset=utf-8;boundary=" + bounary);
        req.set(http::field::content_length, std::to_string(req.body().size()));
        http::write(tcpStream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(tcpStream, buffer, res);
        // swap body data
        data.swap(body);
        //
        beast::error_code rc;
        // tcpStream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, rc);
        tcpStream.shutdown(tcp::socket::shutdown_both, rc);

        if (rc && rc != beast::errc::not_connected)
        {
            throw beast::system_error{rc};
        }

        return res.body();
    }
    catch (std::exception const &e)
    {
        std::cerr << "XHttpHelper::post, error: " << e.what() << std::endl;
        std::cerr << "http host : " << host << " Port : " << port << std::endl;
        return std::string();
    }
    return std::string();
}

std::string XHttpHelper::syncJsonDirectPost(
    const std::string &host,
    const std::string &port,
    const std::string &page,
    std::string &data)
{
    try
    {
        asio::io_context ioCtx;
        asio::ip::tcp::resolver resolver(ioCtx);
        tcp::socket tcpStream{ioCtx};

        auto endPoint = resolver.resolve(host, port);
        asio::connect(tcpStream, endPoint.begin(), endPoint.end());

        http::request<http::string_body> req{http::verb::post, page, 10};
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        std::string &body = req.body();
        body.swap(data);
        req.set(http::field::content_type, "application/json;charset=utf-8");
        req.set(http::field::content_length, std::to_string(req.body().size()));
        http::write(tcpStream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;

        http::read(tcpStream, buffer, res);
        // swap body data
        data.swap(body);
        //
        beast::error_code rc;
        // tcpStream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, rc);
        tcpStream.shutdown(tcp::socket::shutdown_both, rc);

        if (rc && rc != beast::errc::not_connected)
        {
            throw beast::system_error{rc};
        }

        return res.body();
    }
    catch (std::exception const &e)
    {
        std::cerr << "XHttpHelper::post, error: " << e.what() << std::endl;
        std::cerr << "http host : " << host << " Port : " << port << std::endl;
        return std::string();
    }
    return std::string();
}
int XHttpHelper::asyncGet(
    const std::string & /*host*/,
    const std::string & /*port*/,
    const std::string & /*page*/,
    const std::map<std::string, std::string> & /*data*/,
    std::function<void(int, const std::string &)> /*callback*/)
{
    std::cerr << "XHttpHelper::asyncGet not implemented, use XHttpHelper::syncGet instead." << std::endl;
    return -1;
}

int XHttpHelper::asyncPost(
    const std::string & /*host*/,
    const std::string & /*port*/,
    const std::string & /*page*/,
    const std::map<std::string, std::string> & /*data*/,
    std::function<void(int, const std::string &)> /*callback*/)
{
    std::cerr << "XHttpHelper::asyncPost not implemented, use XHttpHelper::syncPost instead." << std::endl;
    return -1;
}
