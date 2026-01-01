#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <variant>

#include "utils.hpp"

enum class ActivationType : uint8_t {
    SILU,
    GELU,
    NotImplemented,
};

struct ModelConfig {
    // Qwen 3 config for now
    int BOS = -1;
    int EOS = -1;
    int head_dim = -1;
    int hidden_size = -1;
    int n_kv_heads = -1;
    int rope_theta = -1;
    int vocab_size = -1;
    int max_pos_embeddings = -1;
    int n_attn_heads = -1;
    int n_hidden_layers = -1;
    int max_window_layers = -1;
    double rms_norm_eps = -1.0;
    ActivationType hidden_act = ActivationType::NotImplemented;
};

std::expected<ModelConfig, FileError> from_file(
    const std::filesystem::path& path);
