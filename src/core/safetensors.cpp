#include "safetensors.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <json.hpp>
#include <algorithm>

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

std::expected<Metadata, FileError> SafeTensors::get_metadata() const {
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


    std::vector<TensorMetadata> tensors;
    tensors.reserve((data.size() - 1));

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
        auto precision = precision_data.value().get<std::string>();

        assert(shape_vec.size() <= 4);
        std::array<int, 4> shape{};
        std::copy(shape_vec.begin(), shape_vec.end(), shape);



        tensors.emplace_back(TensorMetadata {
            .id = key,
            .shape = shape,
            .offset_begin = data_offsets[0],
            .offset_end = data_offsets[1],
            .precision =  str_to_tensor_dtype(precision),
        });

        
    }

    return tensors;
}


