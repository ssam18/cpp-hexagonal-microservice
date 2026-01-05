#include "config/Config.h"
#include "utils/Logger.h"
#include "domain/ProductRepositoryMongo.h"
#include "service/ProductService.h"
#include "adapters/ProductHandler.h"
#include "adapters/HttpServer.h"
#include <mongocxx/instance.hpp>
#include <memory>
#include <iostream>
#include <csignal>

// Global server pointer for signal handling
std::shared_ptr<adapters::HttpServer> g_server;

void signalHandler(int signum) {
    utils::Logger::info("Interrupt signal (" + std::to_string(signum) + ") received. Shutting down...");
    if (g_server) {
        g_server->stop();
    }
    exit(signum);
}

int main() {
    try {
        // Initialize logger
        utils::Logger::init();
        utils::Logger::info("=== Product Catalog Microservice ===");
        utils::Logger::info("Starting application...");

        // Validate configuration
        config::Config::validate();

        // Initialize MongoDB instance (must be done once)
        mongocxx::instance instance{};
        utils::Logger::info("MongoDB C++ driver initialized");

        // Get configuration
        auto mongoUri = config::Config::getMongoUri();
        auto dbName = config::Config::getDatabaseName();
        auto serverAddress = config::Config::getServerAddress();
        auto serverPort = config::Config::getServerPort();

        utils::Logger::info("Configuration:");
        utils::Logger::info("  MongoDB URI: " + mongoUri);
        utils::Logger::info("  Database: " + dbName);
        utils::Logger::info("  Server: " + serverAddress + ":" + std::to_string(serverPort));

        // Wire up dependencies (Dependency Injection)
        // 1. Create repository (Secondary Adapter - outbound)
        auto repository = std::make_shared<domain::ProductRepositoryMongo>(mongoUri, dbName);
        
        // 2. Create service (Business Logic)
        auto service = std::make_shared<service::ProductService>(repository);
        
        // 3. Create handler (Primary Adapter - inbound)
        auto productHandler = std::make_shared<adapters::ProductHandler>(service);
        auto requestHandler = std::make_shared<adapters::RequestHandler>(productHandler);
        
        // 4. Create HTTP server
        g_server = std::make_shared<adapters::HttpServer>(serverAddress, serverPort, requestHandler);

        // Register signal handlers
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        utils::Logger::info("Application started successfully!");
        utils::Logger::info("API Endpoints:");
        utils::Logger::info("  GET    /health");
        utils::Logger::info("  GET    /products");
        utils::Logger::info("  GET    /products/{id}");
        utils::Logger::info("  POST   /products");
        utils::Logger::info("  PUT    /products/{id}");
        utils::Logger::info("  DELETE /products/{id}");

        // Run the server
        g_server->run();

    } catch (const std::exception& e) {
        utils::Logger::error("Fatal error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
