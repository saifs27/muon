#pragma once
#include <immintrin.h>

#include <string_view>
#include <vector>

#include "../backends/cpu/math_cpu.hpp"
#include "config.hpp"
#include "tensor.hpp"
#include "tokenizer.hpp"

struct Weights {
    // currently only supports weights for Qwen 3 0.6B
    // TODO: create graph structure to make adding new models easier

    Tensor<float> input_layernorm;
    Tensor<float> post_attn_layernorm;

    // mlp weights
    Tensor<float> down_proj;
    Tensor<float> gate_proj;
    Tensor<float> up_proj;

    // attention weights
    Tensor<float> k_norm;  // (128)
    Tensor<float> k_proj;  // (1024, 1024)

    Tensor<float> o_proj;  // (1024, 2048)

    Tensor<float> q_norm;
    Tensor<float> q_proj;

    Tensor<float> v_proj;
};

struct Block {};

struct Model {
    ModelConfig config;
    std::vector<Block>& blocks;
    Model(const ModelConfig& config, std::vector<Block>& blocks)
        : config(config), blocks(blocks) {};

    void forward(int token);
    void generate(std::string prompt, int steps);
};
