#include "utils/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace utils {

void Logger::init() {
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
}

void Logger::info(const std::string& message) {
    spdlog::info(message);
}

void Logger::error(const std::string& message) {
    spdlog::error(message);
}

void Logger::warn(const std::string& message) {
    spdlog::warn(message);
}

void Logger::debug(const std::string& message) {
    spdlog::debug(message);
}

} // namespace utils
