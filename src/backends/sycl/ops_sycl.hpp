#pragma once
#include <sycl/sycl.hpp>

namespace sycl_backend {

void matmul(sycl::queue& q, const float* A, const float* B, float* C, int M, int N, int K);
//void moe_gate();
//void rmsnorm();
//void layer_norm();

void softmax(sycl::queue& q, const float* in, float* out, int n);

void gelu(sycl::queue& q);
//void silu();

//void rope();
//void attn();
//void gqa();
//void ffn();
//void forward();





void gqa(float* q_proj,  // [emb_dim, n_heads * head_dim]
         float* k_proj,
         float* v_proj,
         float* o_proj);
}