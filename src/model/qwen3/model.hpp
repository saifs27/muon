#pragma once
#include "core/tensor.hpp"
#include "core/safetensors.hpp"
#include "core/config.hpp"
#include "core/utils.hpp"

#include "block.hpp"

#include <expected>
#include <stdfloat>
#include <array>

namespace qwen3 {

enum class ModelError {
    FailedToLoadWeights,
};

class Model {
    Model(
        const std::array<Block<>, 28> blocks,
        const Tensor<> lm_head,
        const Tensor<> embed_tokens,
        const Tensor<> norm) :
        blocks(blocks),
        lm_head(lm_head),
        embed_tokens(embed_tokens),
        norm(norm)
        {};

public:
    std::array<Block<>, 28> blocks;
    Tensor<> lm_head;
    Tensor<> embed_tokens;
    Tensor<> norm;

    static std::expected<Model,ModelError> load(const SafeTensors& sf);
    void forward(int token);
    void generate(std::string prompt, int steps);
};

}
