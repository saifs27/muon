#include "config.hpp"

#include <fstream>
#include <json.hpp>

std::expected<ModelConfig, FileError> from_file(
    const std::filesystem::path& path) {
    const auto json_data = json::to_json(path);

    if (!json_data.has_value()) {
        return std::unexpected(json_data.error());
    }

    auto get_int = [&json_data](const std::string& id) {
        return json::access(json_data.value(), id).value_or(-1).get<int>();
    };

    auto get_double = [&json_data](const std::string& id) {
        return json::access(json_data.value(), id).value_or(-1).get<double>();
    };

    auto get_act = [&json_data](const std::string& id) {
        auto str =
            json::access(json_data.value(), id).value_or("").get<std::string>();
        if (str == "silu") {
            return ActivationType::SILU;
        }

        if (str == "gelu") {
            return ActivationType::GELU;
        }
        return ActivationType::NotImplemented;
    };

    const int BOS = get_int("bos_token_id");
    const int EOS = get_int("eos_token_id");
    const int head_dim = get_int("head_dim");
    const int hidden_size = get_int("hidden_size");
    const int n_kv_heads = get_int("num_key_value_heads");
    const int rope_theta = get_int("rope_theta");
    const int vocab_size = get_int("vocab_size");
    const int max_pos_embeddings = get_int("max_position_embeddings");
    const int n_attn_heads = get_int("num_attention_heads");
    const int n_hidden_layers = get_int("num_hidden_layers");
    const int max_window_layers = get_int("max_window_layers");

    const ActivationType hidden_act = get_act("hidden_act");
    const double rms_norm_eps = get_double("rms_norm_eps");

    return ModelConfig{
        .BOS = BOS,
        .EOS = EOS,
        .head_dim = head_dim,
        .hidden_size = hidden_size,
        .n_kv_heads = n_kv_heads,
        .rope_theta = rope_theta,
        .vocab_size = vocab_size,
        .max_pos_embeddings = max_pos_embeddings,
        .n_attn_heads = n_attn_heads,
        .n_hidden_layers = n_hidden_layers,
        .max_window_layers = max_window_layers,
        .rms_norm_eps = rms_norm_eps,
        .hidden_act = hidden_act,
    };

}

