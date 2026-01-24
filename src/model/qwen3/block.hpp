#pragma once
#include <optional>
#include <stdfloat>

#include "core/tensor.hpp"
#include "inference.hpp"

template <typename T = std::bfloat16_t>
struct Block {
    Tensor<T> input_layernorm;
    Tensor<T> post_attn_layernorm;

    // mlp weights
    Tensor<T> down_proj;
    Tensor<T> gate_proj;
    Tensor<T> up_proj;

    // attention weights
    Tensor<T> k_norm;  // (128)
    Tensor<T> k_proj;  // (1024, 1024)

    Tensor<T> o_proj;  // (1024, 2048)

    Tensor<T> q_norm;
    Tensor<T> q_proj;

    Tensor<T> v_proj;
    
    Block() = default;
    static std::optional<Block<T>> create(
        std::flat_map<std::string, Tensor<T>> m, const int idx) {
        std::string id = std::to_string(idx);
        
        Tensor<T> _input_layernorm     = m["model.layers." + id + ".input_layernorm.weight"];
        Tensor<T> _post_attn_layernorm = m["model.layers." + id + ".post_attention_layernorm.weight"];

        Tensor<T> _down_proj           = m["model.layers." + id + ".mlp.down_proj.weight"];
        Tensor<T> _gate_proj           = m["model.layers." + id + ".mlp.gate_proj.weight"];
        Tensor<T> _up_proj             = m["model.layers." + id + ".mlp.up_proj.weight"];

        Tensor<T> _k_norm              = m["model.layers." + id + ".self_attn.k_norm.weight"];
        Tensor<T> _k_proj              = m["model.layers." + id + ".self_attn.k_proj.weight"];

        Tensor<T> _o_proj              = m["model.layers." + id + ".self_attn.o_proj.weight"];

        Tensor<T> _q_norm              = m["model.layers." + id + ".self_attn.q_norm.weight"];
        Tensor<T> _q_proj              = m["model.layers." + id + ".self_attn.q_proj.weight"];

        Tensor<T> _v_proj              = m["model.layers." + id + ".self_attn.v_proj.weight"];

        return Block(
            _input_layernorm,
            _post_attn_layernorm,
            _down_proj,
            _gate_proj,
            _up_proj,
            _k_norm,
            _k_proj,
            _o_proj,
            _q_norm,
            _q_proj,
            _v_proj);
    }
    /*
        void block(InferenceState& s) {
            s.x_resid = rmsnorm(s.x, input_layernorm_weight);
            s.x_resid = gqa(
                s.x_resid,
                q_proj, k_proj, v_proj,
                q_norm, k_norm,
                o_proj,
                kv_cache
            );
            s.x += s.x_resid;

            s.x_resid = rms_norm(s.x, post_attn_layernorm);

            s.x_resid = swiglu(s.x_resid, gate_proj, up_proj, down_proj);
            s.x += s.x_resid;

        }
            */



    

private:
    Block(
        Tensor<T> input_layernorm,
        Tensor<T> post_attn_layernorm,
        Tensor<T> down_proj,
        Tensor<T> gate_proj,
        Tensor<T> up_proj,
        Tensor<T> k_norm,
        Tensor<T> k_proj,
        Tensor<T> o_proj,
        Tensor<T> q_norm,
        Tensor<T> q_proj,
        Tensor<T> v_proj) :
        input_layernorm(input_layernorm),
        post_attn_layernorm(post_attn_layernorm),
        down_proj(down_proj),
        gate_proj(gate_proj),
        up_proj(up_proj),
        k_norm(k_norm),
        k_proj(k_proj),
        o_proj(o_proj),
        q_norm(q_norm),
        q_proj(q_proj),
        v_proj(v_proj) {}

};