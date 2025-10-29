#pragma once
#include <memory>

enum class DType {
    fp32,
    fp16,
    bf16,
    i32,
    i16,
    i8,
};

template <DType dtype, size_t dims>
struct Tensor {
    std::unique_ptr<float> data;
    int shape[dims];
    int stride[dims];

    Tensor(float* data, int shape[dims], int stride[dims]);
};
