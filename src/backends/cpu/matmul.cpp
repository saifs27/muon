#include "ops.hpp"
#include <immintrin.h>
#include <omp.h>

void matmul(const Tensor<std::bfloat16_t> x, const Tensor<std::bfloat16_t> w, Tensor<std::bfloat16_t> out) {
    int i;

    auto n = x.size();

}
