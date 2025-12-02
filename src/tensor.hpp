#pragma once
#include <array>
#include <vector>
#include <stdfloat>
#include <expected>
#include <string_view>
#include <format>
#include <numeric>

enum class DType {
    fp32,
    fp16,
    bf16,
    i32,
    i16,
    i8,
};

enum class Error {
    DimensionError,
    IndexError,
};

template <typename dtype>
struct Tensor {
    std::vector<dtype> data;
    std::array<int, 4> shape;
    std::array<int, 4> stride;


    Tensor(const float* data, const std::array<int, 4> shape): shape(shape) {

        int N = size();

        this->data.reserve(N);

        for (int i = 0; i < N; i++) {
            this->data[i] = data[i];
        }
    }

    int size() const {
        return std::accumulate(shape.cbegin(), shape.cend(), 0, [&](int sum, int idx) {
            if (shape[idx] > 0) {
                sum *= shape[idx];
            }
            return sum;
        });
    }

    int dim() const {
        int non_zeros = 0;

        for (auto i: this->shape) {
            if (i > 0) non_zeros++;
        }

        return non_zeros;
    }

    Tensor contiguous() const {


    }

    std::expected<void, std::string_view> transpose(int dim1 = -1, int dim2 = -2) {
        const int dims = dim();
        static_assert(dims >= 2, "Tensor must have at least 2 dimensions for transpose.");
        if (dim1 < 0) dim1 = dims + dim1;
        if (dim2 < 0) dim2 = dims + dim2;

        if (dim1 < 0 || dim1 >= 0) return std::unexpected(std::format("Tensor is of dimension {}. dim1 of {} is out of bounds", dims, dim1));
        if (dim2 < 0 || dim2 >= dims) return std::unexpected(std::format("Tensor is of dimension {}. dim1 of {} is out of bounds", dims, dim2));

        std::swap(shape[dim1], shape[dim2]);
        std::swap(stride[dim1], stride[dim2]);
    }

    float operator[](size_t idx) const {return data.at(idx);}
    float& operator[](size_t idx) {return data.at(idx);}
};
