#include "ops.hpp"
#include <immintrin.h>
#include <omp.h>
#include <cmath>
#include <vector>
#include <ranges>
#include <span>


void rope(
    float* output,
    const float * input,
    const int batch,
    const int seq_len,
    const int n_heads,
    const int head_dim
    ) {
    int half_dim = head_dim / 2;
    std::vector<float> cos_buffer(half_dim);
    std::vector<float> sin_buffer(half_dim);

    auto get_freqs = [&](int m, float* out_cost, float* out_sin) {

    };


}
