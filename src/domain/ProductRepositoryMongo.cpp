#include "domain/ProductRepositoryMongo.h"
#include "utils/Logger.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/oid.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;

namespace domain {

ProductRepositoryMongo::ProductRepositoryMongo(const std::string& connectionString,
                                               const std::string& databaseName)
    : client_(mongocxx::uri{connectionString}), db_(client_[databaseName]) {
    utils::Logger::info("Connected to MongoDB database: " + databaseName);
}

std::pair<std::vector<Product>, std::optional<utils::AppError>> 
ProductRepositoryMongo::findAll(const std::string& category) {
    try {
        auto collection = db_["products"];
        std::vector<Product> products;

        document filter_builder{};
        if (!category.empty()) {
            filter_builder << "category" << category;
        }

        auto cursor = collection.find(filter_builder.view());
        
        for (auto&& doc : cursor) {
            products.push_back(documentToProduct(doc));
        }

        utils::Logger::info("Found " + std::to_string(products.size()) + " products");
        return {products, std::nullopt};
    } catch (const mongocxx::exception& e) {
        utils::Logger::error("MongoDB error in findAll: " + std::string(e.what()));
        return {{}, utils::AppError::internalError("Database error occurred")};
    }
}

std::pair<std::optional<Product>, std::optional<utils::AppError>> 
ProductRepositoryMongo::findById(const std::string& id) {
    try {
        auto collection = db_["products"];
        
        document filter_builder{};
        filter_builder << "_id" << bsoncxx::oid(id);

        auto result = collection.find_one(filter_builder.view());
        
        if (result) {
            Product product = documentToProduct(result->view());
            return {product, std::nullopt};
        } else {
            return {std::nullopt, utils::AppError::notFound("Product not found")};
        }
    } catch (const std::exception& e) {
        utils::Logger::error("Error in findById: " + std::string(e.what()));
        return {std::nullopt, utils::AppError::internalError("Database error occurred")};
    }
}

std::pair<std::string, std::optional<utils::AppError>> 
ProductRepositoryMongo::create(const Product& product) {
    try {
        auto collection = db_["products"];
        
        auto doc = productToDocument(product);
        auto result = collection.insert_one(doc.view());
        
        if (result) {
            auto id = result->inserted_id().get_oid().value.to_string();
            utils::Logger::info("Created product with ID: " + id);
            return {id, std::nullopt};
        } else {
            return {"", utils::AppError::internalError("Failed to create product")};
        }
    } catch (const mongocxx::exception& e) {
        utils::Logger::error("MongoDB error in create: " + std::string(e.what()));
        return {"", utils::AppError::internalError("Database error occurred")};
    }
}

std::optional<utils::AppError> 
ProductRepositoryMongo::update(const Product& product) {
    try {
        auto collection = db_["products"];
        
        document filter_builder{};
        filter_builder << "_id" << bsoncxx::oid(product.getId());

        document update_builder{};
        update_builder << "$set" << open_document
                      << "name" << product.getName()
                      << "description" << product.getDescription()
                      << "price" << product.getPrice()
                      << "stock" << product.getStock()
                      << "category" << product.getCategory()
                      << close_document;

        auto result = collection.update_one(filter_builder.view(), update_builder.view());
        
        if (result && result->matched_count() > 0) {
            utils::Logger::info("Updated product: " + product.getId());
            return std::nullopt;
        } else {
            return utils::AppError::notFound("Product not found");
        }
    } catch (const mongocxx::exception& e) {
        utils::Logger::error("MongoDB error in update: " + std::string(e.what()));
        return utils::AppError::internalError("Database error occurred");
    }
}

std::optional<utils::AppError> 
ProductRepositoryMongo::deleteById(const std::string& id) {
    try {
        auto collection = db_["products"];
        
        document filter_builder{};
        filter_builder << "_id" << bsoncxx::oid(id);

        auto result = collection.delete_one(filter_builder.view());
        
        if (result && result->deleted_count() > 0) {
            utils::Logger::info("Deleted product: " + id);
            return std::nullopt;
        } else {
            return utils::AppError::notFound("Product not found");
        }
    } catch (const mongocxx::exception& e) {
        utils::Logger::error("MongoDB error in deleteById: " + std::string(e.what()));
        return utils::AppError::internalError("Database error occurred");
    }
}

bool ProductRepositoryMongo::exists(const std::string& id) {
    try {
        auto collection = db_["products"];
        
        document filter_builder{};
        filter_builder << "_id" << bsoncxx::oid(id);

        auto count = collection.count_documents(filter_builder.view());
        return count > 0;
    } catch (const mongocxx::exception& e) {
        utils::Logger::error("MongoDB error in exists: " + std::string(e.what()));
        return false;
    }
}

Product ProductRepositoryMongo::documentToProduct(const bsoncxx::document::view& doc) {
    Product product;
    
    product.setId(doc["_id"].get_oid().value.to_string());
    product.setName(std::string(doc["name"].get_string().value));
    product.setDescription(std::string(doc["description"].get_string().value));
    
    // Handle price as either double or int32
    auto price_element = doc["price"];
    if (price_element.type() == bsoncxx::type::k_double) {
        product.setPrice(price_element.get_double().value);
    } else if (price_element.type() == bsoncxx::type::k_int32) {
        product.setPrice(static_cast<double>(price_element.get_int32().value));
    } else if (price_element.type() == bsoncxx::type::k_int64) {
        product.setPrice(static_cast<double>(price_element.get_int64().value));
    }
    
    product.setStock(doc["stock"].get_int32().value);
    product.setCategory(std::string(doc["category"].get_string().value));
    
    return product;
}

bsoncxx::document::value ProductRepositoryMongo::productToDocument(const Product& product) {
    document doc{};
    
    if (!product.getId().empty()) {
        doc << "_id" << bsoncxx::oid(product.getId());
    }
    
    doc << "name" << product.getName()
        << "description" << product.getDescription()
        << "price" << product.getPrice()
        << "stock" << product.getStock()
        << "category" << product.getCategory();
    
    return doc << finalize;
}

} // namespace domain
