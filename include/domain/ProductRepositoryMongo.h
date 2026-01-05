#pragma once

#include "domain/ProductRepository.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

namespace domain {

/**
 * ProductRepositoryMongo - Secondary Adapter
 * Implements ProductRepository interface using MongoDB
 */
class ProductRepositoryMongo : public ProductRepository {
public:
    explicit ProductRepositoryMongo(const std::string& connectionString,
                                   const std::string& databaseName);

    std::pair<std::vector<Product>, std::optional<utils::AppError>> 
        findAll(const std::string& category = "") override;

    std::pair<std::optional<Product>, std::optional<utils::AppError>> 
        findById(const std::string& id) override;

    std::pair<std::string, std::optional<utils::AppError>> 
        create(const Product& product) override;

    std::optional<utils::AppError> 
        update(const Product& product) override;

    std::optional<utils::AppError> 
        deleteById(const std::string& id) override;

    bool exists(const std::string& id) override;

private:
    mongocxx::client client_;
    mongocxx::database db_;
    
    Product documentToProduct(const bsoncxx::document::view& doc);
    bsoncxx::document::value productToDocument(const Product& product);
};

} // namespace domain
