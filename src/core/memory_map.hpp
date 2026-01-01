#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <span>
#include <string_view>

#include "utils.hpp"

struct MemoryMap {
    [[nodiscard]]
    static std::expected<MemoryMap, FileError> map(const std::filesystem::path& file_path);
    void unmap(); 

    ~MemoryMap() {unmap();}
    MemoryMap(const MemoryMap&) = delete;
    MemoryMap& operator=(const MemoryMap&) = delete;
    MemoryMap(MemoryMap&& other) noexcept;
    MemoryMap& operator=(MemoryMap&& other) noexcept;

    [[nodiscard]] std::span<std::byte> view_data() const noexcept {
        return std::span<std::byte>(static_cast<std::byte*>(m.data), m.length / 8);
    }

    [[nodiscard]] size_t size() const noexcept { return m.length; }

private:
    struct M {
        void * data = nullptr;
        uintmax_t length = 0;
    } m;

    explicit MemoryMap(M m) : m(std::move(m)){};
};
