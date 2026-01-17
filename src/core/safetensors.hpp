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
    [[nodiscard]] std::expected<Metadata, FileError> get_metadata() const;

    std::byte * data() const {return m.map.data();}

    std::span<std::byte> view_range(size_t start, size_t end) const & {
        auto data = m.map.view_data();

        assert(end > start);
        assert( end <= m.map.size());


        return data.subspan(start, end - start);

    }
};