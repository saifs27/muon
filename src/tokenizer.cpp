#include "tokenizer.hpp"
#include <json.hpp>
#include <fstream>
#include <numeric>
#include <ranges>
#include <algorithm>


Tokenizer::Tokenizer(int vocab_size) : vocab_size(vocab_size) {
    vocab.resize(vocab_size);
}

std::string Tokenizer::decode(const std::vector<int>& tokens) const {
    std::string res;
    int string_size = std::accumulate(tokens.cbegin(), tokens.cend(), 0, 
        [&](int sum, int idx) {
            sum += vocab[idx].size();
            return sum;
    });

    std::cout << tokens.size() << '\n';

    res.reserve(string_size);
    std::cout << string_size << '\n';

    for (int i = 0, size = tokens.size(); i < size; i++) {
        std::cout << vocab[tokens[i]] << '\n';
        res.append(vocab[tokens[i]]);
    }

    return res;
};

std::expected<void, FileError> Tokenizer::from_file(const std::filesystem::path& file_path) {
    using json = nlohmann::json;
    
    std::ifstream file(file_path);
    auto data = json::parse(file);
    auto vocab_list = data.at("model").at("vocab");
    auto added_tokens = data.at("added_tokens");

    auto is_in_range = [&](const auto& idx) {return idx >= 0 && idx < vocab_size;};

    for (auto token : vocab_list.items()) {
        if (!is_in_range(token.value())) return std::unexpected(FileError::UnexpectedData);
        vocab[token.value()] = token.key();
    }

    for (const auto& item : added_tokens) {
        if (!is_in_range(item.at("id"))) return std::unexpected(FileError::UnexpectedData);
        vocab[item.at("id")] = item.at("content");
    }

    return {};

}

