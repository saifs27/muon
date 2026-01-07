#pragma once
#include <array>
#include <cstdint>
#include <expected>
#include <numeric>
#include <string>
#include <vector>
#include <string_view>

enum class DType : uint8_t {
    fp32,
    fp16,
    bf16,
    i32,
    i16,
    i8,
    invalid
};

inline DType str_to_tensor_dtype(const std::string_view dtype) {
    if (dtype == "FP32") {return DType::fp32;}
    if (dtype == "FP16") {return DType::fp16;}
    if (dtype == "BF16") {return DType::bf16;}
    if (dtype == "I32") {return DType::i32;}
    if (dtype == "I16") {return DType::i16;}
    if (dtype == "I8") {return DType::i8;}

    return DType::invalid;

}

enum class TensorError : uint8_t {
    DimensionError,
    IndexError,
};

struct TensorMetadata {
    std::string id = "";
    std::vector<int> shape {0, 0, 0, 0};
    size_t offset_begin = 0;
    size_t offset_end = 0;
    DType precision = DType::bf16;
};

using Metadata = std::vector<TensorMetadata>;

template <typename T>
struct Tensor {
    std::array<int, 4> shape;
    std::array<int, 4> stride;
    std::vector<T> data;
    int offset_begin;
    int offset_end;
    DType precision;

    Tensor(const float* data, const std::array<int, 4> shape) : shape(shape) {}

    T at(int idx) const {}

    T at(const int i, const int j) const {
        int idx = i * stride.at(0) + j * stride.at(1);
        return data.at(idx);
    }

    T at(int i, int j, int k) const;
    T at(int i, int j, int k, int m) const;

    int size() const { return data.size(); }

    int dim() const {
        auto non_zeros = 0;

        for (auto i : this->shape) {
            if (i > 0) non_zeros++;
        }

        return non_zeros;
    }

    Tensor contiguous() const {}

    [[nodiscard]]
    std::expected<void, TensorError> transpose(int dim1 = -1, int dim2 = -2) {
        const auto dims = dim();
        if (dims < 2) return std::unexpected(TensorError::DimensionError);

        if (dim1 < 0) dim1 = dims + dim1;
        if (dim2 < 0) dim2 = dims + dim2;

        if (dim1 < 0 || dim1 >= dims)
            return std::unexpected(TensorError::DimensionError);
        if (dim2 < 0 || dim2 >= dims)
            return std::unexpected(TensorError::DimensionError);

        std::swap(shape[dim1], shape[dim2]);
        std::swap(stride[dim1], stride[dim2]);
    }

    float operator[](size_t idx) const { return data.at(idx); }
    float& operator[](size_t idx) { return data.at(idx); }
};
