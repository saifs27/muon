#pragma once

#include <expected>
#include <filesystem>
#include <map>

#include "memory_map.hpp"
#include "tensor.hpp"
#include "utils.hpp"

/*
Temporarily using map due to need for ordering and spotty compiler support for flat_map
TODO: use more cache-friendly option like flat_map for better performance.
*/
using weights_map = std::map<std::string, Tensor<>>; 
// 

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

    // returns weights grouped together by layer (note: sorting is lexicographic by default (i.e sorted like: layer 1, 10, 11)).
    [[nodiscard]] std::expected<std::vector<weights_map>, FileError> get_weights_by_layer() const; 
    std::byte * data() const {return m.map.data();}

    std::span<std::byte> view_range(size_t start, size_t end) const & {
        auto data = m.map.view_data();
        assert(end > start);
        assert( end <= m.map.size());

        return data.subspan(start, end - start);

    }
};

inline int get_layer_idx(std::string_view id) {
    auto digits = id
        | std::views::split('.')
        | std::views::filter([](auto&& part){
            return std::ranges::all_of(part, [](unsigned char c) {
                return std::isdigit(c);
            });
        });

        if (std::ranges::distance(digits) != 1) {return -1;}
        const auto digit_str = std::ranges::to<std::vector<std::string>>(digits);
        return std::stoi(digit_str[0]);
    };

