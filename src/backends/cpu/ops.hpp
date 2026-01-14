#pragma once
#include <immintrin.h>
#include <omp.h>

#include <cmath>
#include <stdfloat>

#include "../../core/tensor.hpp"

template <typename T>
void matmul(Tensor<T>& result, const Tensor<T>& A, const Tensor<T>& B);

template <typename T>
void silu(const Tensor<T>& x, Tensor<T>& output) {
    for (int i = 0; i < x.size(); i++) {
        output[i] = x.at(i) / (1.0f + expf(-x.at(i)));
    }
}

template <typename T>
void softmax(Tensor<T>& x) {
    int size = x.size();
    float max_value = x[0];
    for (int i = 0; i < size; i++) {
        if (x[i] > max_value) max_value = i;
    }

    // sum all values
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += std::exp(max_value - x[i]);
    }

    for (int i = 0; i < size; i++) {
        x[i] = std::exp(max_value - x[i]) / sum;
    }
}

template <typename T>
void rms_norm(const Tensor<T>& x, const Tensor<T>& w, Tensor<T>& y, float eps,
              int size) {
    float sum = 0;

    for (int i = 0; i < size; i++) {
        sum += x[i] * x[i];
    }

    float rms = sqrt(eps + sum / size);

    for (int i = 0; i < size; i++) {
        y[i] = (x[i] / rms) * w[i];
    }
}

template <typename T>
void layer_norm(const Tensor<T>& x, const Tensor<T>& w, T bias, float eps,
                Tensor<T>& output) {
    T expected;
    T var;

    T mean = [&]() {

    };

    for (int i = 0, size = x.size(); i < size; i++) {
        output[i] = (x) / sqrt(x);
    }
}

template <typename T>
void rope(Tensor<T> vec);

template <typename T>
void linear(Tensor<T> x, Tensor<T>& w, float b);

template <typename T>
void gqa(Tensor<T> q_proj,  // [emb_dim, n_heads * head_dim]
         Tensor<T> k_proj, Tensor<T> v_proj, Tensor<T> o_proj);
