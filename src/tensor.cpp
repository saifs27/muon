#include "tensor.hpp"

template <DType dtype, size_t dims>
Tensor<dtype, dims>::Tensor(float* data, int shape[dims], int stride[dims])
    : shape(shape), stride(stride) {}
