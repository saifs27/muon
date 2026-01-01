#pragma once

#include <expected>
#include <filesystem>

#include "memory_map.hpp"
#include "tensor.hpp"
#include "utils.hpp"

class SafeTensors {
    struct M {
        MemoryMap map;
        size_t header_size = 0;
    } m;

    explicit SafeTensors(M src) : m(std::move(src)) {};

   public:
    [[nodiscard]]
    static std::expected<SafeTensors, FileError> load(
        const std::filesystem::path& path);

    [[nodiscard]] size_t header_size() const noexcept { return m.header_size; }
    [[nodiscard]] std::expected<std::string, FileError> read_header() const;
    [[nodiscard]] std::expected<Tensor<float>, FileError> get_tensor() const;
};

[[nodiscard]]
std::expected<std::string, FileError> read_header(
    const std::filesystem::path& path);
