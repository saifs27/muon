#pragma once
#include <array>
#include <cstdint>
#include <expected>
#include <numeric>
#include <string>
#include <vector>

enum class DType : uint8_t {
    fp32,
    fp16,
    bf16,
    i32,
    i16,
    i8,
};

enum class TensorError : uint8_t {
    DimensionError,
    IndexError,
};

struct TensorMetadata {
    std::string id;
    std::array<int, 4> shape{};
    size_t offset_begin;
    size_t offset_end;
    DType precision;
};

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
