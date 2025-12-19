#pragma once
#include <stdfloat>


#include "tensor.hpp"

struct Shape {
    int x = 0;
    int y = 0;
    int z = 0;
};

template <typename T>
void matmul(Tensor<T>& result, Tensor<T> A, Tensor<T> B);


void softmax(Tensor<float> x);

void gemm(const Tensor<float>& A, const Tensor<float>& B, Tensor<float>& C);

void swish(Tensor<float>& output, const Tensor<float>& input);

void rms_norm(Tensor<float> input);
void layer_norm();
void rope();


template <typename T>
void linear(Tensor<T> x, Tensor<T>& w, float b);

void gqa(
    Tensor<float> q_proj, // [emb_dim, n_heads * head_dim]
    Tensor<float> k_proj,
    Tensor<float> v_proj,
    Tensor<float> o_proj
    );