#pragma once

#include <string_view>
#include <filesystem>
#include <expected>
#include "tensor.hpp"
#include "utils.hpp"




namespace sf {

[[nodiscard]]
std::expected<std::string, FileError> read_header(const std::filesystem::path& path);

std::string print_tensors(const std::string_view sf_json);
std::string get_tensors(const std::string_view sf_json);





} // namespace sf
