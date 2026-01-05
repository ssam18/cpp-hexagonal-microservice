#include "utils/JsonUtils.h"

namespace utils {

nlohmann::json JsonUtils::parse(const std::string& str) {
    return nlohmann::json::parse(str);
}

std::string JsonUtils::stringify(const nlohmann::json& json) {
    return json.dump();
}

} // namespace utils
