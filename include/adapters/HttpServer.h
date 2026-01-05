#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace adapters {

class RequestHandler;

/**
 * HttpServer - Primary Adapter
 * Handles incoming HTTP requests using Boost.Beast
 */
class HttpServer {
public:
    HttpServer(const std::string& address, unsigned short port,
               std::shared_ptr<RequestHandler> handler);
    
    void run();
    void stop();

private:
    std::string address_;
    unsigned short port_;
    std::shared_ptr<RequestHandler> handler_;
    boost::asio::io_context ioc_;
    
    void doAccept();
};

} // namespace adapters
