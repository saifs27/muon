#pragma once

#include "tensor.hpp"

struct QwenConfig {
    int BOS = -1;
    int EOS = -1;
    int head_dim;
    int hidden_size;
    int n_kv_heads;
    int rope_theta;
    int vocab_size;
    int max_pos_embeddings;
    int n_attn_heads;
    int n_hidden_layers;
    ActivationType hidden_act;
};

struct Weights {
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
    QwenConfig config;
    std::vector<Block>& blocks;
    Model(Config config, std::vector<Block>& blocks)
        : config(config), blocks(blocks) {};

    void forward(int token);
    void generate(std::string prompt, int steps);
};
