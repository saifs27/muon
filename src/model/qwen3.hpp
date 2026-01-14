#pragma once
#include "../core/tensor.hpp"
#include "../core/tokenizer.hpp"
#include "../core/safetensors.hpp"
#include "../core/config.hpp"
#include "../core/utils.hpp"

#include <expected>

namespace qwen3 {

enum class ModelError {
    FailedToLoadWeights,

};

struct Block {
    Tensor<bf16> input_layernorm;
    Tensor<bf16> post_attn_layernorm;

    // mlp weights
    Tensor<bf16> down_proj;
    Tensor<bf16> gate_proj;
    Tensor<bf16> up_proj;

    // attention weights
    Tensor<bf16> k_norm;  // (128)
    Tensor<bf16> k_proj;  // (1024, 1024)

    Tensor<bf16> o_proj;  // (1024, 2048)

    Tensor<bf16> q_norm;
    Tensor<bf16> q_proj;

    Tensor<bf16> v_proj;

    void set_weight(std::string id, Tensor<bf16> weight);

};

class Model { 
    std::vector<Block>& blocks;
    Model(std::vector<Block>& blocks)
        : blocks(blocks) {};

    public:
    static std::expected<Model,ModelError> load(const SafeTensors& sf);

    void forward(int token);
    void generate(std::string prompt, int steps);
};

}
