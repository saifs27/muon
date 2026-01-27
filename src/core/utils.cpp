#include "utils.hpp"
#include "memory_map.hpp"
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
    auto map = MemoryMap::map(path);

    if (!map.has_value()) {
        return std::unexpected(map.error());
    }

    const auto& mmap = map.value();
    auto view = mmap.view_data();

    const char* begin = std::bit_cast<const char*>(view.data());
    const char* end = begin + view.size();

    auto json_data = nlohmann::json::parse(begin, end, nullptr, false);

    if (json_data.is_discarded()) {
        return std::unexpected(FileError::JsonParseFailed);
    }
    return json_data;
}

std::expected<std::reference_wrapper<const nlohmann::json>, FileError> access(
    const nlohmann::json& json, const std::string_view id) {
    auto it = json.find(id);
    if (it == json.end()) {
        return std::unexpected(FileError::JsonUnexpectedData);
    }

    return *it;
}
std::expected<std::reference_wrapper<const nlohmann::json>, FileError>
access_nested(const nlohmann::json& json, const std::string_view id1,
              const std::string_view id2) {
    auto it1 = json.find(id1);
    if (it1 == json.end()) {
        return std::unexpected(FileError::JsonUnexpectedData);
    }
    const auto& nested_json = *it1;
    auto it2 = nested_json.find(id2);

    if (it2 == nested_json.end()) {
        return std::unexpected(FileError::JsonUnexpectedData);
    }

    return *it2;
}

}  // namespace json