#pragma once
#include <array>
#include <cmath>
#include <cstdint>

#include "tensor.hpp"

struct Shape {
    int x = 0;
    int y = 0;
    int z = 0;
};

template <size_t M, size_t N, size_t K>
void matmul(std::array<float, M * N>& result, std::array<float, M * K> A,
            std::array<float, K * N> B);

template <size_t n>
void softmax(std::array<float, n> x);

template <size_t n>
void rms_norm(std::array<float, n> input);
void layer_norm();
void rope();
void attn();

void block();
