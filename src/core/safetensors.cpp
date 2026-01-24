#include "safetensors.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <json.hpp>
#include <algorithm>
#include <ranges>

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


std::expected<std::string, FileError> SafeTensors::read_header() const {
    auto bytes = m.map.view_data();

    if (bytes.size() < (8 + m.header_size)) {
        return std::unexpected(FileError::ReadFailed);
    }

    const char* data_ptr = std::bit_cast<const char*>(bytes.data());
    std::string header(data_ptr + 8, m.header_size);
    return header;
}

std::expected<std::vector<weights_map>, FileError>
SafeTensors::get_weights_by_layer() const {
    // read header
    auto bytes = m.map.view_data();

    if (bytes.size() < (8 + m.header_size)) {
        return std::unexpected(FileError::ReadFailed);
    }

    const auto data_ptr = std::bit_cast<const char*>(bytes.data());
    const std::string header(data_ptr + 8, m.header_size);
    
    
    // parse header
    auto data = nlohmann::json::parse(header, nullptr, false);

    if (data.is_discarded()) {
        return std::unexpected(FileError::JsonParseFailed);
    }

    auto cmp = [&](auto& a, auto& b) {
        return get_layer_idx(a) < get_layer_idx(b);
    };

    std::flat_map<std::string, Tensor<>> tensors;

    for (auto& [key, value] : data.items()) {
        if (key == "__metadata__") {
            continue;
        }

        auto shape_data = json::access(value, "shape");
        auto offset_data = json::access(value, "data_offsets");
        auto precision_data = json::access(value, "dtype");

        if (!shape_data.has_value() ||!offset_data.has_value() || !precision_data.has_value()) {
            return std::unexpected(FileError::JsonAccessFailed);
        }

        auto data_offsets = offset_data.value().get<std::array<uint64_t, 2>>();
        auto shape_vec = shape_data.value().get<std::vector<int>>();
        auto precision =
            str_to_tensor_dtype(precision_data.value().get<std::string>());

        assert(shape_vec.size() <= 4);
        std::array<int, 4> shape{};
        size_t copy_count = std::min(shape_vec.size(), shape.size());
        std::ranges::copy_n(shape_vec.begin(), copy_count, shape.begin());

        auto start = 8 + header_size();
        auto bytes =
            view_range(start + data_offsets[0], start + data_offsets[1]);
        tensors[key] = Tensor<>(bytes, shape);
    }

    auto grouped_weights =
        tensors | std::views::chunk_by([&](const auto& a, const auto& b) {
            return get_layer_idx(a.first) == get_layer_idx(b.first);
        }) |
        std::ranges::to<std::vector<std::flat_map<std::string, Tensor<>>>>();

    return grouped_weights;
}


