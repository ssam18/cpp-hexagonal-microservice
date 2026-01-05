#include "service/ProductService.h"
#include "utils/Logger.h"

namespace service {

ProductService::ProductService(std::shared_ptr<domain::ProductRepository> repository)
    : repository_(std::move(repository)) {}

std::pair<std::vector<dto::ProductResponse>, std::optional<utils::AppError>>
ProductService::getAllProducts(const std::string& category) {
    utils::Logger::info("Getting all products" + 
                       (category.empty() ? "" : " for category: " + category));
    
    auto [products, error] = repository_->findAll(category);
    
    if (error) {
        return {{}, error};
    }
    
    std::vector<dto::ProductResponse> response;
    for (const auto& product : products) {
        response.push_back(productToDto(product));
    }
    
    return {response, std::nullopt};
}

std::pair<std::optional<dto::ProductResponse>, std::optional<utils::AppError>>
ProductService::getProduct(const std::string& id) {
    utils::Logger::info("Getting product: " + id);
    
    auto [product, error] = repository_->findById(id);
    
    if (error) {
        return {std::nullopt, error};
    }
    
    if (!product) {
        return {std::nullopt, utils::AppError::notFound("Product not found")};
    }
    
    return {productToDto(*product), std::nullopt};
}

std::pair<dto::ProductResponse, std::optional<utils::AppError>>
ProductService::createProduct(const dto::CreateProductRequest& request) {
    utils::Logger::info("Creating product: " + request.name);
    
    // Validate request
    if (!request.isValid()) {
        return {{}, utils::AppError::badRequest("Invalid product data")};
    }
    
    // Create domain entity
    domain::Product product("", request.name, request.description,
                           request.price, request.stock, request.category);
    
    // Save to repository
    auto [id, error] = repository_->create(product);
    
    if (error) {
        return {{}, error};
    }
    
    // Fetch the created product
    product.setId(id);
    
    return {productToDto(product), std::nullopt};
}

std::pair<dto::ProductResponse, std::optional<utils::AppError>>
ProductService::updateProduct(const dto::UpdateProductRequest& request) {
    utils::Logger::info("Updating product: " + request.id);
    
    // Validate request
    if (!request.isValid()) {
        return {{}, utils::AppError::badRequest("Invalid product data")};
    }
    
    // Check if product exists
    if (!repository_->exists(request.id)) {
        return {{}, utils::AppError::notFound("Product not found")};
    }
    
    // Create updated domain entity
    domain::Product product(request.id, request.name, request.description,
                           request.price, request.stock, request.category);
    
    // Update in repository
    auto error = repository_->update(product);
    
    if (error) {
        return {{}, error};
    }
    
    return {productToDto(product), std::nullopt};
}

std::optional<utils::AppError>
ProductService::deleteProduct(const std::string& id) {
    utils::Logger::info("Deleting product: " + id);
    
    return repository_->deleteById(id);
}

dto::ProductResponse ProductService::productToDto(const domain::Product& product) {
    dto::ProductResponse response;
    response.id = product.getId();
    response.name = product.getName();
    response.description = product.getDescription();
    response.price = product.getPrice();
    response.stock = product.getStock();
    response.category = product.getCategory();
    response.status = product.getStatus();
    return response;
}

} // namespace service
