#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace utils {

/**
 * JSON utility functions
 */
class JsonUtils {
public:
    static nlohmann::json parse(const std::string& str);
    static std::string stringify(const nlohmann::json& json);
};

} // namespace utils
