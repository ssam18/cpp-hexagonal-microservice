#pragma once

#include <string>
#include <memory>
#include <optional>

namespace domain {

/**
 * Product entity - Core domain model
 * This represents a product in our catalog
 */
class Product {
public:
    Product() = default;
    Product(const std::string& id, const std::string& name, 
            const std::string& description, double price, int stock,
            const std::string& category);

    // Getters
    std::string getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getDescription() const { return description_; }
    double getPrice() const { return price_; }
    int getStock() const { return stock_; }
    std::string getCategory() const { return category_; }
    std::string getStatus() const;

    // Setters
    void setId(const std::string& id) { id_ = id; }
    void setName(const std::string& name) { name_ = name; }
    void setDescription(const std::string& description) { description_ = description; }
    void setPrice(double price) { price_ = price; }
    void setStock(int stock) { stock_ = stock; }
    void setCategory(const std::string& category) { category_ = category; }

    // Business logic
    bool isAvailable() const { return stock_ > 0; }
    bool canFulfillOrder(int quantity) const { return stock_ >= quantity; }
    void reduceStock(int quantity);
    void increaseStock(int quantity);

private:
    std::string id_;
    std::string name_;
    std::string description_;
    double price_{0.0};
    int stock_{0};
    std::string category_;
};

} // namespace domain
