#pragma once
#include <array>
#include <cstdint>
#include <expected>
#include <numeric>
#include <string>
#include <vector>
#include <string_view>
#include <span>
#include "types.hpp"
#include <memory>
#include <flat_map>
#include <stdfloat>
#include <charconv>
#include <map>

enum class DType : uint8_t {
    fp32,
    fp16,
    bf16,
    i8,
    invalid
};

inline constexpr int dtype_to_n_bytes(DType dtype) {
    switch (dtype) {
        case DType::fp32 : return 4;
        case DType::fp16 : return 2;
        case DType::bf16 : return 2;
        case DType::i8 : return 1;
        case DType::invalid : return 0;
    }
};

inline DType str_to_tensor_dtype(const std::string_view dtype) {
    if (dtype == "FP32") {return DType::fp32;}
    if (dtype == "FP16") {return DType::fp16;}
    if (dtype == "BF16") {return DType::bf16;}
    if (dtype == "I8") {return DType::i8;}

    return DType::invalid;

}

enum class TensorError : uint8_t {
    DimensionError,
    IndexError,
};


template <typename T = std::bfloat16_t>
struct Tensor {
    std::span<T> data {};
    std::array<int, 4> shape {};

    Tensor() = default;

    // constructor for accepting raw bytes from safetensors file
    Tensor(const std::span<std::byte> raw_bytes, const std::array<int, 4> shape)
        : shape(shape) {
            auto count = raw_bytes.size_bytes() / sizeof(T);
            data = std::span<T>(std::bit_cast<T*>(raw_bytes.data()), count);
        }


    Tensor(const std::span<T> data, std::array<int, 4> shape) :
        data(data), shape(shape) {}


    std::span<T> view() const {
        return data;
    }

    int64_t size() const {
        return data.size();
    }

};

struct TensorMetadata {
    std::array<int, 4> shape {0, 0, 0, 0};
    size_t offset_begin = 0;
    size_t offset_end = 0;
    DType precision = DType::bf16;
};

inline int get_layer_idx(std::string_view id) {
    auto digits = id 
        | std::views::split('.')
        | std::views::filter([](auto&& part){
            return std::ranges::all_of(part, [](unsigned char c) {
                return std::isdigit(c);
            });
        })
        | std::ranges::to<std::vector<std::string>>();

        if (digits.size() != 1) {return -1;}
        return std::stoi(digits[0]);
    };

using Metadata = std::flat_map<std::string, TensorMetadata>;



inline std::vector<Metadata> group_metadata_by_layer(const Metadata& meta, int n_layers) {
    auto grouped_meta = meta 
        | std::views::chunk_by([&](const auto& a, const auto& b) { return get_layer_idx(a.first) == get_layer_idx(b.first); })
        | std::ranges::to<std::vector<Metadata>>();


    return grouped_meta; 
}
