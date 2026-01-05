#pragma once

#include <string>
#include <cstdlib>

namespace config {

/**
 * Configuration class
 * Reads configuration from environment variables
 */
class Config {
public:
    static std::string getServerAddress() {
        return getEnv("SERVER_ADDRESS", "0.0.0.0");
    }
    
    static unsigned short getServerPort() {
        return std::stoi(getEnv("SERVER_PORT", "8080"));
    }
    
    static std::string getMongoUri() {
        return getEnv("MONGO_URI", "mongodb://localhost:27017");
    }
    
    static std::string getDatabaseName() {
        return getEnv("DATABASE_NAME", "product_catalog");
    }
    
    static void validate() {
        // Ensure required environment variables are set
        getServerAddress();
        getServerPort();
        getMongoUri();
        getDatabaseName();
    }

private:
    static std::string getEnv(const std::string& key, const std::string& defaultValue = "") {
        const char* val = std::getenv(key.c_str());
        return val ? std::string(val) : defaultValue;
    }
};

} // namespace config
