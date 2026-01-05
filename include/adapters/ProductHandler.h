#pragma once

#include "service/ProductService.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;

namespace adapters {

/**
 * ProductHandler - Primary Adapter
 * Handles HTTP requests for product operations
 */
class ProductHandler {
public:
    explicit ProductHandler(std::shared_ptr<service::ProductService> service);

    // Handle HTTP request
    http::response<http::string_body> 
        handleRequest(const http::request<http::string_body>& req);

private:
    std::shared_ptr<service::ProductService> service_;

    // Route handlers
    http::response<http::string_body> handleGetAllProducts(const http::request<http::string_body>& req);
    http::response<http::string_body> handleGetProduct(const std::string& id);
    http::response<http::string_body> handleCreateProduct(const http::request<http::string_body>& req);
    http::response<http::string_body> handleUpdateProduct(const std::string& id, 
                                                          const http::request<http::string_body>& req);
    http::response<http::string_body> handleDeleteProduct(const std::string& id);

    // Helper methods
    http::response<http::string_body> createResponse(http::status status, 
                                                     const std::string& body);
    http::response<http::string_body> createJsonResponse(http::status status, 
                                                         const nlohmann::json& json);
    http::response<http::string_body> createErrorResponse(int code, 
                                                          const std::string& message);
    std::string extractIdFromPath(const std::string& path);
    std::string extractQueryParam(const std::string& target, const std::string& param);
};

/**
 * RequestHandler - Routes requests to appropriate handlers
 */
class RequestHandler {
public:
    explicit RequestHandler(std::shared_ptr<ProductHandler> productHandler);

    http::response<http::string_body> 
        handle(const http::request<http::string_body>& req);

private:
    std::shared_ptr<ProductHandler> productHandler_;
};

} // namespace adapters
