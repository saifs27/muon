#include "tokenizer.hpp"

#include <algorithm>
#include <json.hpp>
#include <numeric>
#include <ranges>

#include "utils.hpp"

std::expected<Tokenizer, FileError> Tokenizer::load(
    const ModelConfig& config, const std::filesystem::path& path) {
    std::vector<std::string> vocab;
    vocab.resize(config.vocab_size);

    auto json = json::to_json(path);

    if (!json.has_value()) {
        return std::unexpected(json.error());
    }

    auto vocab_list = json::access_nested(json.value(), "model", "vocab");
    auto added_tokens = json::access(json.value(), "added_tokens");

    if (!vocab_list.has_value() || !added_tokens.has_value()) {
        return std::unexpected(FileError::JsonAccessFailed);
    }

    auto is_in_range = [&](const auto& idx) {
        return idx >= 0 && idx < config.vocab_size;
    };

    for (const auto token : vocab_list.value().items()) {
        if (!is_in_range(token.value()))
            return std::unexpected(FileError::UnexpectedData);
        vocab[token.value()] = token.key();
    }

    for (const auto& item : added_tokens.value()) {
        if (!is_in_range(item.at("id")))
            return std::unexpected(FileError::UnexpectedData);
        vocab[item.at("id")] = item.at("content");
    }

    return Tokenizer(M{
        .vocab = vocab,
        .vocab_size = config.vocab_size,
        .BOS = config.BOS,
        .EOS = config.EOS,
    });
}

std::string Tokenizer::decode(const std::vector<int>& tokens) const {
    std::string res;
    const int string_size = std::accumulate(tokens.cbegin(), tokens.cend(), 0,
                                            [&](int sum, const int idx) {
                                                sum += m.vocab[idx].size();
                                                return sum;
                                            });

    res.reserve(string_size);

    for (const auto token : tokens) {
        res.append(m.vocab[token]);
    }

    return res;
};
