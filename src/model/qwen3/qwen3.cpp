#include "qwen3.hpp"
#include <ranges>
#include <algorithm>
#include <vector>
#include <optional>
#include <charconv>
#include <cassert>

namespace qwen3 {

std::expected<Model,ModelError> Model::load(const SafeTensors& sf) {
    auto metadata = sf.get_metadata();
    if (!metadata.has_value()) {
        return std::unexpected(ModelError::FailedToLoadWeights);
    }
    const auto metadata_map = metadata.value();
    
    auto get_weight = [&](const auto& pair) {
        const auto start = sf.header_size() + 8;
        const auto& m = pair.second;
        auto weight_span = sf.view_range(start + m.offset_begin, start + m.offset_end);
        return Tensor<std::bfloat16_t>(weight_span, m.shape);
    };

    std::array<Block<>, 28> blocks{};
    auto grouped_metadata = group_metadata_by_layer(metadata_map, 28);
    
    Tensor<> lm_head;
    Tensor<> embed_tokens;
    Tensor<> norm;
    bool set_lm_head_and_embed_tokens = false;
    bool set_norm = false;

    for (auto&& chunk : grouped_metadata) {
        int idx = get_layer_idx(chunk.keys().at(0));

        auto weights = chunk 
            | std::views::transform(get_weight)
            | std::ranges::to<std::vector<Tensor<>>>();

        auto layer = std::views::zip(chunk.keys(), weights)
            | std::ranges::to<std::flat_map<std::string, Tensor<>>>();

        if (layer.size() == 2) {
            lm_head = layer["lm_head.weight"];
            embed_tokens = layer["model.embed_tokens.weight"];
            set_lm_head_and_embed_tokens = true;
            continue;
        };
        
        if (layer.size() == 1) {
            norm = layer["model.norm.weight"];
            set_norm = true;
            continue;
        }

        auto block = Block<>::create(layer, idx);

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


void Model::forward(int token) {

}





}
