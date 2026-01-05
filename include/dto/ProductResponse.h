#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace dto {

/**
 * ProductResponse DTO
 * Data Transfer Object for sending product information to clients
 */
struct ProductResponse {
    std::string id;
    std::string name;
    std::string description;
    double price;
    int stock;
    std::string category;
    std::string status;

    // Convert to JSON
    nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id},
            {"name", name},
            {"description", description},
            {"price", price},
            {"stock", stock},
            {"category", category},
            {"status", status}
        };
    }
};

/**
 * CreateProductRequest DTO
 * Data Transfer Object for creating a new product
 */
struct CreateProductRequest {
    std::string name;
    std::string description;
    double price;
    int stock;
    std::string category;

    // Parse from JSON
    static CreateProductRequest fromJson(const nlohmann::json& j) {
        CreateProductRequest req;
        req.name = j.value("name", "");
        req.description = j.value("description", "");
        req.price = j.value("price", 0.0);
        req.stock = j.value("stock", 0);
        req.category = j.value("category", "");
        return req;
    }

    // Validate request
    bool isValid() const {
        return !name.empty() && price >= 0 && stock >= 0 && !category.empty();
    }
};

/**
 * UpdateProductRequest DTO
 * Data Transfer Object for updating an existing product
 */
struct UpdateProductRequest {
    std::string id;
    std::string name;
    std::string description;
    double price;
    int stock;
    std::string category;

    static UpdateProductRequest fromJson(const nlohmann::json& j, const std::string& productId) {
        UpdateProductRequest req;
        req.id = productId;
        req.name = j.value("name", "");
        req.description = j.value("description", "");
        req.price = j.value("price", 0.0);
        req.stock = j.value("stock", 0);
        req.category = j.value("category", "");
        return req;
    }

    bool isValid() const {
        return !id.empty() && !name.empty() && price >= 0 && stock >= 0 && !category.empty();
    }
};

/**
 * ErrorResponse DTO
 * Data Transfer Object for error responses
 */
struct ErrorResponse {
    int code;
    std::string message;

    nlohmann::json toJson() const {
        return nlohmann::json{
            {"code", code},
            {"message", message}
        };
    }
};

} // namespace dto
