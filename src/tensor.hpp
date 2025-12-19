#pragma once
#include <array>
#include <vector>
#include <numeric>
#include <expected>



enum class DType {
    fp32,
    fp16,
    bf16,
    i32,
    i16,
    i8,
};

enum class TensorError {
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
        auto non_zeros = 0;

        for (auto i: this->shape) {
            if (i > 0) non_zeros++;
        }

        return non_zeros;
    }

    Tensor contiguous() const {


    }

    [[nodiscard]]
    std::expected<void, TensorError> transpose(int dim1 = -1, int dim2 = -2) {
        const auto dims = dim();
        if (dims < 2) return std::unexpected(TensorError::DimensionError);

        if (dim1 < 0) dim1 = dims + dim1;
        if (dim2 < 0) dim2 = dims + dim2;

        if (dim1 < 0 || dim1 >= 0) return std::unexpected(TensorError::DimensionError);
        if (dim2 < 0 || dim2 >= dims) return std::unexpected(TensorError::DimensionError);

        std::swap(shape[dim1], shape[dim2]);
        std::swap(stride[dim1], stride[dim2]);
    }

    float operator[](size_t idx) const {return data.at(idx);}
    float& operator[](size_t idx) {return data.at(idx);}
};
