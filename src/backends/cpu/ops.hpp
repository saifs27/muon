#pragma once
#include <immintrin.h>
#include <omp.h>

#include <cmath>
#include <stdfloat>
#include <cstdint>
#include <span>
#include "core/types.hpp"
#include "core/tensor.hpp"
#include <stdfloat>

// activations

constexpr void silu(const Tensor<std::bfloat16_t> x, Tensor<std::bfloat16_t> output) {

}

// ----------------------------------------------------

void matmul(const Tensor<std::bfloat16_t> x, const Tensor<std::bfloat16_t> w, Tensor<std::bfloat16_t> out);

void softmax(Tensor<std::bfloat16_t> x);
void rms_norm(
    const Tensor<std::bfloat16_t> x,
    const Tensor<std::bfloat16_t> w,
    Tensor<std::bfloat16_t> y,
    float eps);

void layer_norm(
    Tensor<std::bfloat16_t> x,
    Tensor<std::bfloat16_t> w,
    float bias,
    float eps,
    Tensor<std::bfloat16_t> output);

void rope(
    Tensor<std::bfloat16_t> output,
    Tensor<std::bfloat16_t> input
    );


void rope_qk(
    Tensor<std::bfloat16_t> Q, // (batch, head_dim, seq_len)
    Tensor<std::bfloat16_t> K);


void gqa(Tensor<std::bfloat16_t> q_proj,  // (emb_dim, n_heads * head_dim)
         Tensor<std::bfloat16_t> k_proj,
         Tensor<std::bfloat16_t> v_proj,
         Tensor<std::bfloat16_t> o_proj);
