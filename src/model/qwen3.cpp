#include "qwen3.hpp"
#include <ranges>
#include <algorithm>
#include <cassert>

namespace qwen3 {

void Block::set_weight(std::string id, Tensor<bf16> weight) {
if (id == "model.layers.0.input_layernorm.weight") {
    input_layernorm = weight;
    return;
}
		
if (id == "model.layers.0.mlp.down_proj.weight") {
    down_proj = weight;
    return;
}

if (id == "model.layers.0.mlp.gate_proj.weight") {
    gate_proj = weight;
    return;
}

if (id == "model.layers.0.mlp.up_proj.weight") {
    up_proj = weight;
    return;
}

if (id == "model.layers.0.post_attention_layernorm.weight") {
    post_attn_layernorm = weight;
    return;
}

if (id == "model.layers.0.self_attn.k_norm.weight") {
    k_norm = weight;
    return;
}

if (id == "model.layers.0.self_attn.k_proj.weight") {
    k_proj = weight;
    return;
}

if (id == "model.layers.0.self_attn.o_proj.weight") {
    o_proj = weight;
    return;
}

if (id == "model.layers.0.self_attn.q_norm.weight") {
    q_norm = weight;
    return;
}

if (id == "model.layers.0.self_attn.q_proj.weight") {
    q_proj = weight;
    return;
}

if (id == "model.layers.0.self_attn.v_proj.weight") {
    v_proj = weight;
    return;
}

return;

}

 std::expected<Model,ModelError> Model::load(const SafeTensors& sf) {
    auto metadata = sf.get_metadata();
    if (!metadata.has_value()) {
        return std::unexpected(ModelError::FailedToLoadWeights);
    }

    auto start = sf.header_size() + 8;

    auto get_weight = [&](TensorMetadata m) {
        auto weight_span = sf.view_range(start + m.offset_begin, start + m.offset_end);
        return Tensor<bf16>(weight_span, m);
    };

    auto get_block = [&](std::string_view name){};

    for (auto weight : metadata.value()) {

        
    }






 }


void Model::forward(int token) {

}





}
