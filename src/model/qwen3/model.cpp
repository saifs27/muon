#include "model.hpp"
#include <ranges>
#include <algorithm>
#include <vector>
#include <optional>
#include <charconv>
#include <cassert>

namespace qwen3 {

std::expected<Model,ModelError> Model::load(const SafeTensors& sf) {
    auto weights = sf.get_weights_by_layer();
    if (!weights.has_value()) {
        return std::unexpected(ModelError::FailedToLoadWeights);
    }
    auto& layers = weights.value();
    
    std::array<Block<>, 28> blocks{};
    
    Tensor<> lm_head;
    Tensor<> embed_tokens;
    Tensor<> norm;
    bool set_lm_head_and_embed_tokens = false;
    bool set_norm = false;

    for (auto&& weights_map : layers) {
        int idx = get_layer_idx(weights_map.keys().at(0));

        if (weights_map.size() == 2) {
            lm_head = weights_map["lm_head.weight"];
            embed_tokens = weights_map["model.embed_tokens.weight"];
            set_lm_head_and_embed_tokens = true;
            continue;
        };
        
        if (weights_map.size() == 1) {
            norm = weights_map["model.norm.weight"];
            set_norm = true;
            continue;
        }

        auto block = Block<>::create(weights_map, idx);

        if (!block.has_value()) {
            return std::unexpected(ModelError::FailedToLoadWeights);
        }
        
        blocks[idx] = block.value();
    }
    if (!set_lm_head_and_embed_tokens || !set_norm) {
        return std::unexpected(ModelError::FailedToLoadWeights);
    }
    
    return Model(blocks, lm_head, embed_tokens, norm);
}

/*
void Model::forward(int token, InferenceState& s) {
    auto x = embedding_lookup(token, embed_tokens);

    for (auto& b : blocks) { 
        b.block(s)
    }

    s.x = rmsnorm(s.x, norm);
    s.logits = linear(s.x, lm_head);

}
*/





}
