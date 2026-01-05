#include "domain/Product.h"
#include <stdexcept>

namespace domain {

Product::Product(const std::string& id, const std::string& name,
                 const std::string& description, double price, int stock,
                 const std::string& category)
    : id_(id), name_(name), description_(description),
      price_(price), stock_(stock), category_(category) {}

std::string Product::getStatus() const {
    if (stock_ > 10) {
        return "in-stock";
    } else if (stock_ > 0) {
        return "low-stock";
    } else {
        return "out-of-stock";
    }
}

void Product::reduceStock(int quantity) {
    if (quantity < 0) {
        throw std::invalid_argument("Quantity cannot be negative");
    }
    if (stock_ < quantity) {
        throw std::invalid_argument("Insufficient stock");
    }
    stock_ -= quantity;
}

void Product::increaseStock(int quantity) {
    if (quantity < 0) {
        throw std::invalid_argument("Quantity cannot be negative");
    }
    stock_ += quantity;
}

} // namespace domain
