#pragma once

#include <expected>
#include <filesystem>
#include <json.hpp>

enum class FileError {
    FileNotFound,
    ReadFailed,
    OpenFailed,
    SeekFailed,
    UnexpectedData,
    FileEmpty,
    FileTooLarge,
    MappingFailed,
    JsonParseFailed,
    JsonUnexpectedData,
    JsonAccessFailed,
};

// helper functions around json library to use std::expected for error handling
// instead of throwing exceptions
namespace json {
std::expected<nlohmann::basic_json<>, FileError> to_json(
    const std::filesystem::path& path);

std::expected<nlohmann::json, FileError> access(const nlohmann::json& json,
                                                const std::string& id);

std::expected<nlohmann::json, FileError> access_nested(
    const nlohmann::json& json, const std::string& id1, const std::string& id2);
}  // namespace json
