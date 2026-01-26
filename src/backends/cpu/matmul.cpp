#include "ops.hpp"
#include <immintrin.h>
#include <omp.h>

void matmul(const Tensor<> x, const Tensor<> w, Tensor<> out) {
    int i;

    auto n = x.size();

}
