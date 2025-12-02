#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

#include "tensor.hpp"

struct Shape {
    int x = 0;
    int y = 0;
    int z = 0;
};

template <typename dtype>
void matmul(Tensor<dtype>& result, Tensor<dtype> A, Tensor<dtype> B);


void softmax(Tensor<float> x);

template <typename dtype>
Tensor<dtype> linear(Tensor<dtype> x, Tensor<dtype> w, float b);

template <size_t n>
void rms_norm(Tensor<float> input);
void layer_norm();
void rope();
void attention();

void block();
