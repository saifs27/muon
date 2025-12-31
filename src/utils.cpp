#include "utils.hpp"

#include <fstream>
namespace json {
std::expected<nlohmann::basic_json<>, FileError> to_json(
    const std::filesystem::path& path) {
    std::ifstream file(path);
    auto json_data = nlohmann::json::parse(file, nullptr, false);

    if (json_data.is_discarded()) {
        return std::unexpected(FileError::JsonParseFailed);
    }
    return json_data;
}

std::expected<nlohmann::json, FileError> access(const nlohmann::json& json,
                                                const std::string& id) {
    auto it = json.find(id);
    if (it == json.end()) {
        return std::unexpected(FileError::JsonUnexpectedData);
    }

    return *it;
}
std::expected<nlohmann::json, FileError> access_nested(
    const nlohmann::json& json, const std::string& id1,
    const std::string& id2) {
    return access(json, id1).and_then(
        [&](const auto& j) { return access(j, id2); });
}

}  // namespace json