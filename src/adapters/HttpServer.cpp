#include "adapters/HttpServer.h"
#include "adapters/ProductHandler.h"
#include "utils/Logger.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <thread>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace adapters {

// HTTP session class
class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket, std::shared_ptr<RequestHandler> handler)
        : socket_(std::move(socket)), handler_(handler) {}

    void run() {
        doRead();
    }

private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::shared_ptr<RequestHandler> handler_;

    void doRead() {
        auto self = shared_from_this();
        http::async_read(socket_, buffer_, req_,
            [self](beast::error_code ec, std::size_t) {
                if (!ec) {
                    self->handleRequest();
                } else {
                    utils::Logger::error("Read error: " + ec.message());
                }
            });
    }

    void handleRequest() {
        auto response = handler_->handle(req_);
        
        auto self = shared_from_this();
        http::async_write(socket_, response,
            [self](beast::error_code ec, std::size_t) {
                if (ec) {
                    utils::Logger::error("Write error: " + ec.message());
                }
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            });
    }
};

// Listener class
class Listener : public std::enable_shared_from_this<Listener> {
public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint,
             std::shared_ptr<RequestHandler> handler)
        : ioc_(ioc), acceptor_(ioc), handler_(handler) {
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        if (ec) {
            utils::Logger::error("Open error: " + ec.message());
            return;
        }

        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec) {
            utils::Logger::error("Set option error: " + ec.message());
            return;
        }

        acceptor_.bind(endpoint, ec);
        if (ec) {
            utils::Logger::error("Bind error: " + ec.message());
            return;
        }

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            utils::Logger::error("Listen error: " + ec.message());
            return;
        }
    }

    void run() {
        doAccept();
    }

private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<RequestHandler> handler_;

    void doAccept() {
        acceptor_.async_accept(
            [self = shared_from_this()](beast::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<HttpSession>(std::move(socket), self->handler_)->run();
                }
                self->doAccept();
            });
    }
};

HttpServer::HttpServer(const std::string& address, unsigned short port,
                       std::shared_ptr<RequestHandler> handler)
    : address_(address), port_(port), handler_(handler) {}

void HttpServer::run() {
    auto const address = net::ip::make_address(address_);
    auto const endpoint = tcp::endpoint{address, port_};

    utils::Logger::info("Starting HTTP server on " + address_ + ":" + std::to_string(port_));

    std::make_shared<Listener>(ioc_, endpoint, handler_)->run();

    ioc_.run();
}

void HttpServer::stop() {
    ioc_.stop();
}

} // namespace adapters
