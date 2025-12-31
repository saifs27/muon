#include "safetensors.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <fstream>
#include <json.hpp>

std::expected<SafeTensors, FileError> SafeTensors::load(
    const std::filesystem::path& path) {
    auto map = MemoryMap::map(path);

    if (!map.has_value()) {
        return std::unexpected(map.error());
    }

    auto bytes = map.value().view_data();
    size_t header_size = 0;

    for (int i = 0; i < 8; i++) {
        header_size |= static_cast<uint64_t>(bytes[i]) << (i * 8);
    }

    return SafeTensors(M{
        .map = std::move(map.value()),
        .header_size = header_size,
    });
}


[[nodiscard]]
std::expected<std::string, FileError> read_header(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
        return std::unexpected(FileError::FileNotFound);
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

