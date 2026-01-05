#include "adapters/ProductHandler.h"
#include "utils/Logger.h"
#include <nlohmann/json.hpp>
#include <regex>

namespace adapters {

ProductHandler::ProductHandler(std::shared_ptr<service::ProductService> service)
    : service_(service) {}

http::response<http::string_body> 
ProductHandler::handleRequest(const http::request<http::string_body>& req) {
    std::string path(req.target());
    auto method = req.method();

    utils::Logger::info(std::string(http::to_string(method)) + " " + path);

    // Remove query parameters from path for routing
    size_t queryPos = path.find('?');
    std::string routePath = (queryPos != std::string::npos) ? path.substr(0, queryPos) : path;

    // Route: GET /products
    if (method == http::verb::get && routePath == "/products") {
        return handleGetAllProducts(req);
    }
    
    // Route: GET /products/{id}
    if (method == http::verb::get && routePath.find("/products/") == 0) {
        std::string id = extractIdFromPath(routePath);
        if (!id.empty()) {
            return handleGetProduct(id);
        }
    }
    
    // Route: POST /products
    if (method == http::verb::post && routePath == "/products") {
        return handleCreateProduct(req);
    }
    
    // Route: PUT /products/{id}
    if (method == http::verb::put && routePath.find("/products/") == 0) {
        std::string id = extractIdFromPath(routePath);
        if (!id.empty()) {
            return handleUpdateProduct(id, req);
        }
    }
    
    // Route: DELETE /products/{id}
    if (method == http::verb::delete_ && routePath.find("/products/") == 0) {
        std::string id = extractIdFromPath(routePath);
        if (!id.empty()) {
            return handleDeleteProduct(id);
        }
    }
    
    // Route: GET /health
    if (method == http::verb::get && routePath == "/health") {
        nlohmann::json health = {{"status", "healthy"}, {"service", "product-catalog"}};
        return createJsonResponse(http::status::ok, health);
    }

    return createErrorResponse(404, "Not Found");
}

http::response<http::string_body> 
ProductHandler::handleGetAllProducts(const http::request<http::string_body>& req) {
    std::string category = extractQueryParam(std::string(req.target()), "category");
    
    auto [products, error] = service_->getAllProducts(category);
    
    if (error) {
        return createErrorResponse(error->getHttpCode(), error->getMessage());
    }
    
    nlohmann::json jsonArray = nlohmann::json::array();
    for (const auto& product : products) {
        jsonArray.push_back(product.toJson());
    }
    
    return createJsonResponse(http::status::ok, jsonArray);
}

http::response<http::string_body> 
ProductHandler::handleGetProduct(const std::string& id) {
    auto [product, error] = service_->getProduct(id);
    
    if (error) {
        return createErrorResponse(error->getHttpCode(), error->getMessage());
    }
    
    if (!product) {
        return createErrorResponse(404, "Product not found");
    }
    
    return createJsonResponse(http::status::ok, product->toJson());
}

http::response<http::string_body> 
ProductHandler::handleCreateProduct(const http::request<http::string_body>& req) {
    try {
        auto json = nlohmann::json::parse(req.body());
        auto request = dto::CreateProductRequest::fromJson(json);
        
        auto [product, error] = service_->createProduct(request);
        
        if (error) {
            return createErrorResponse(error->getHttpCode(), error->getMessage());
        }
        
        return createJsonResponse(http::status::created, product.toJson());
    } catch (const std::exception& e) {
        return createErrorResponse(400, "Invalid JSON: " + std::string(e.what()));
    }
}

http::response<http::string_body> 
ProductHandler::handleUpdateProduct(const std::string& id, 
                                    const http::request<http::string_body>& req) {
    try {
        auto json = nlohmann::json::parse(req.body());
        auto request = dto::UpdateProductRequest::fromJson(json, id);
        
        auto [product, error] = service_->updateProduct(request);
        
        if (error) {
            return createErrorResponse(error->getHttpCode(), error->getMessage());
        }
        
        return createJsonResponse(http::status::ok, product.toJson());
    } catch (const std::exception& e) {
        return createErrorResponse(400, "Invalid JSON: " + std::string(e.what()));
    }
}

http::response<http::string_body> 
ProductHandler::handleDeleteProduct(const std::string& id) {
    auto error = service_->deleteProduct(id);
    
    if (error) {
        return createErrorResponse(error->getHttpCode(), error->getMessage());
    }
    
    nlohmann::json response = {{"message", "Product deleted successfully"}};
    return createJsonResponse(http::status::ok, response);
}

http::response<http::string_body> 
ProductHandler::createResponse(http::status status, const std::string& body) {
    http::response<http::string_body> res{status, 11};
    res.set(http::field::content_type, "text/plain");
    res.body() = body;
    res.prepare_payload();
    return res;
}

http::response<http::string_body> 
ProductHandler::createJsonResponse(http::status status, const nlohmann::json& json) {
    http::response<http::string_body> res{status, 11};
    res.set(http::field::content_type, "application/json");
    res.body() = json.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body> 
ProductHandler::createErrorResponse(int code, const std::string& message) {
    dto::ErrorResponse error{code, message};
    return createJsonResponse(static_cast<http::status>(code), error.toJson());
}

std::string ProductHandler::extractIdFromPath(const std::string& path) {
    std::regex pattern("/products/([a-fA-F0-9]{24})");
    std::smatch matches;
    if (std::regex_match(path, matches, pattern) && matches.size() > 1) {
        return matches[1].str();
    }
    return "";
}

std::string ProductHandler::extractQueryParam(const std::string& target, 
                                              const std::string& param) {
    size_t queryPos = target.find('?');
    if (queryPos == std::string::npos) {
        return "";
    }
    
    std::string query = target.substr(queryPos + 1);
    std::string searchParam = param + "=";
    size_t paramPos = query.find(searchParam);
    
    if (paramPos == std::string::npos) {
        return "";
    }
    
    size_t startPos = paramPos + searchParam.length();
    size_t endPos = query.find('&', startPos);
    
    if (endPos == std::string::npos) {
        return query.substr(startPos);
    } else {
        return query.substr(startPos, endPos - startPos);
    }
}

// RequestHandler implementation
RequestHandler::RequestHandler(std::shared_ptr<ProductHandler> productHandler)
    : productHandler_(productHandler) {}

http::response<http::string_body> 
RequestHandler::handle(const http::request<http::string_body>& req) {
    return productHandler_->handleRequest(req);
}

} // namespace adapters
