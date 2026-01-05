#pragma once

#include <string>
#include <spdlog/spdlog.h>

namespace utils {

/**
 * Logger utility class
 * Wrapper around spdlog for logging
 */
class Logger {
public:
    static void init();
    
    static void info(const std::string& message);
    static void error(const std::string& message);
    static void warn(const std::string& message);
    static void debug(const std::string& message);
};

} // namespace utils
