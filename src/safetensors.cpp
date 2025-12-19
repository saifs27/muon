#include "safetensors.hpp"

#include <json.hpp>
#include <bit>
#include <cstdint>
#include <fstream>
#include <array>


namespace sf {

[[nodiscard]]
std::expected<std::string, FileError> read_header(const std::filesystem::path& path) {

    if (!std::filesystem::exists(path)) return std::unexpected(FileError::FileNotFound);
    std::ifstream file(path, std::ios::binary);
    if (!file) return std::unexpected(FileError::OpenFailed);

    // Read first 8 bytes to get header size
    std::array<std::uint8_t, 8> buffer;

    if (!file.read(std::bit_cast<char*>(buffer.data()), buffer.size())) {
        return std::unexpected(FileError::ReadFailed);
    }

    auto header_size = std::bit_cast<std::uint64_t>(buffer);

    // Read header
    std::string header_json;
    
    header_json.resize_and_overwrite(header_size, [&](char* buffer, size_t n) {
        file.read(buffer, n);
        return file.gcount();
    });

    if (file.gcount() != static_cast<std::streamsize>(header_size)) {
        return std::unexpected(FileError::ReadFailed);
    }

    return header_json;
}


std::string get_tensors(const std::string_view sf_json) {
    using json = nlohmann::json;
    json j = json::parse(sf_json.begin(), sf_json.end());
    std::string s = j["lm_head.bias"]["dtype"];
    return s;
}







} //namespace sp