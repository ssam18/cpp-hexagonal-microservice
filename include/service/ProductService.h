#pragma once

#include "domain/ProductRepository.h"
#include "dto/ProductResponse.h"
#include <memory>
#include <vector>

namespace service {

/**
 * ProductService - Application Service Layer
 * Contains business logic and orchestrates domain operations
 */
class ProductService {
public:
    explicit ProductService(std::shared_ptr<domain::ProductRepository> repository);

    // Get all products with optional category filter
    std::pair<std::vector<dto::ProductResponse>, std::optional<utils::AppError>>
        getAllProducts(const std::string& category = "");

    // Get product by ID
    std::pair<std::optional<dto::ProductResponse>, std::optional<utils::AppError>>
        getProduct(const std::string& id);

    // Create new product
    std::pair<dto::ProductResponse, std::optional<utils::AppError>>
        createProduct(const dto::CreateProductRequest& request);

    // Update existing product
    std::pair<dto::ProductResponse, std::optional<utils::AppError>>
        updateProduct(const dto::UpdateProductRequest& request);

    // Delete product
    std::optional<utils::AppError>
        deleteProduct(const std::string& id);

private:
    std::shared_ptr<domain::ProductRepository> repository_;
    
    dto::ProductResponse productToDto(const domain::Product& product);
};

} // namespace service
