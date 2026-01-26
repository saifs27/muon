#pragma once
#include <immintrin.h>
#include <omp.h>


#include <stdfloat>
#include "core/tensor.hpp"
#include <stdfloat>

// activations

constexpr void silu(const Tensor<> x, Tensor<> output);


// ----------------------------------------------------

void matmul(const Tensor<> x, const Tensor<> w, Tensor<> out);

void softmax(Tensor<> x);
void rms_norm(
    const Tensor<> x,
    const Tensor<> w,
    Tensor<> y,
    float eps);

void layer_norm(
    Tensor<> x,
    Tensor<> w,
    float bias,
    float eps,
    Tensor<> output);

void rope(
    Tensor<> output,
    Tensor<> input
    );


void rope_qk(
    Tensor<> Q, // (batch, head_dim, seq_len)
    Tensor<> K);


void gqa(Tensor<> q_proj,  // (emb_dim, n_heads * head_dim)
         Tensor<> k_proj,
         Tensor<> v_proj,
         Tensor<> o_proj);
