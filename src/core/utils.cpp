#include "utils.hpp"
#include <fstream>

std::string file_err_to_string(const FileError err) {
    switch(err) {
        case FileError::FileNotFound : return "File not found";
        case FileError::ReadFailed : return "Failed to read file";
        case FileError::OpenFailed : return "Failed to open file";
        case FileError::SeekFailed : return "Failed to seek file";
        case FileError::UnexpectedData : return "Unexpected data in file";
        case FileError::FileEmpty : return "File is empty";
        case FileError::FileTooLarge : return "File is too large";
        case FileError::MappingFailed : return "Mapping of file failed";
        case FileError::JsonParseFailed : return "Json parsing of file failed";
        case FileError::JsonUnexpectedData : return "Unexpected data in json file";
        case FileError::JsonAccessFailed : return "Failed to access element in json file";
    };

    return "Unknown error";
}

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