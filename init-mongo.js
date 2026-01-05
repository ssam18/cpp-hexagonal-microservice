// MongoDB initialization script
// This script creates sample data for testing

db = db.getSiblingDB('product_catalog');

// Create products collection with sample data
db.products.insertMany([
    {
        name: "Wireless Mouse",
        description: "Ergonomic wireless mouse with USB receiver",
        price: 29.99,
        stock: 150,
        category: "Electronics"
    },
    {
        name: "Mechanical Keyboard",
        description: "RGB mechanical keyboard with blue switches",
        price: 89.99,
        stock: 75,
        category: "Electronics"
    },
    {
        name: "USB-C Cable",
        description: "High-speed USB-C to USB-C cable, 2 meters",
        price: 12.99,
        stock: 500,
        category: "Accessories"
    },
    {
        name: "Laptop Stand",
        description: "Adjustable aluminum laptop stand",
        price: 45.00,
        stock: 8,
        category: "Accessories"
    },
    {
        name: "Webcam HD",
        description: "1080p HD webcam with built-in microphone",
        price: 69.99,
        stock: 0,
        category: "Electronics"
    }
]);

// Create index on category for better query performance
db.products.createIndex({ category: 1 });

print("Database initialized with sample products!");
