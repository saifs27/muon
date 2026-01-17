#pragma once
#include "core/tensor.hpp"

template <typename T>
class InferenceState {
public:
    Tensor<T> x;         // (dim,) - latest activation
    Tensor<T> xb;        // (dim,) - activation inside residual branch
    Tensor<T> xb2;       // (dim,) - activation inside residual branch

    Tensor<T> hb;        // (hidden_dim,) - buffer for hidden dimension in ffn
    Tensor<T> hb2;       // (hidden_dim,) - buffer for hidden dimension in ffn

    Tensor<T> q;         // (n_heads * head_dim,)
    Tensor<T> k;         // (n_kv_heads * head_dim,)
    Tensor<T> v;         // (n_kv_heads * head_dim,)
    Tensor<T> attn;      // (n_heads, seq_len)
    // TODO: add MoE weights

    Tensor<T> logits;    // (vocab_size,)


    
};