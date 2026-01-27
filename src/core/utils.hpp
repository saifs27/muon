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

std::string file_err_to_string(FileError err);

// helper functions around json library to use std::expected for error handling
// instead of throwing exceptions
namespace json {
std::expected<nlohmann::basic_json<>, FileError> to_json(
    const std::filesystem::path& path);

std::expected<std::reference_wrapper<const nlohmann::json>, FileError> access(const nlohmann::json& json,
                                                const std::string_view id);

std::expected<std::reference_wrapper<const nlohmann::json>, FileError> access_nested(
    const nlohmann::json& json, const std::string_view id1, const std::string_view id2);
}  // namespace json
