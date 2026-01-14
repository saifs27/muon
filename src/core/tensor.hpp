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

struct TensorMetadata {
    std::string id = "";
    std::array<int, 4> shape {0, 0, 0, 0};
    size_t offset_begin = 0;
    size_t offset_end = 0;
    DType precision = DType::bf16;
};

using Metadata = std::vector<TensorMetadata>;

template <typename T>
struct Tensor {
    std::span<std::byte> data;
    std::array<int, 4> shape {0, 0, 0, 0};    

    Tensor(const std::span<std::byte> data, const TensorMetadata metadata) 
        : data(data), shape(metadata.shape) {}

    int n_bytes() const {
        if constexpr (T == bf16) {return 2;}
        if constexpr (T == int8_t) {return 1;}
    }
    int64_t size() {
        auto bytes = data.size_bytes();
        auto dtype_size = dtype_to_n_bytes(metadata.precision);
        assert(bytes > 0);
        assert (dtype_size != 0);
        return bytes / dtype_size;
    }

};

