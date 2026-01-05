#pragma once

#include "Product.h"
#include "../dto/ProductResponse.h"
#include "../utils/AppError.h"
#include <vector>
#include <memory>
#include <optional>

namespace domain {

/**
 * ProductRepository interface - Port (Primary)
 * This is the interface that the domain layer expects
 * Implementations are provided by the infrastructure layer (adapters)
 */
class ProductRepository {
public:
    virtual ~ProductRepository() = default;

    // Find all products with optional category filter
    virtual std::pair<std::vector<Product>, std::optional<utils::AppError>> 
        findAll(const std::string& category = "") = 0;

    // Find product by ID
    virtual std::pair<std::optional<Product>, std::optional<utils::AppError>> 
        findById(const std::string& id) = 0;

    // Create new product
    virtual std::pair<std::string, std::optional<utils::AppError>> 
        create(const Product& product) = 0;

    // Update existing product
    virtual std::optional<utils::AppError> 
        update(const Product& product) = 0;

    // Delete product by ID
    virtual std::optional<utils::AppError> 
        deleteById(const std::string& id) = 0;

    // Check if product exists
    virtual bool exists(const std::string& id) = 0;
};

} // namespace domain
