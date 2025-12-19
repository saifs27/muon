#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <expected>

#include <iostream>

#include "utils.hpp"

struct Token {
    char str;
    int id;
};

struct TrieNode {
    std::unordered_map<Token, TrieNode> children;
    bool terminal = false;
};

struct Trie {
    std::unique_ptr<TrieNode> root;
};

struct Tokenizer {
    std::vector<std::string> vocab;
    int vocab_size;
    int BOS = -1;
    int EOS = -1;
    int EOT = -1;
    int byte_fallbacks = -1;

    Tokenizer(int vocab_size);

    std::vector<int> encode(const std::string text) const;
    std::string decode(const std::vector<int>& tokens) const;
    std::string decode_one(int token) const;

    [[nodiscard]]
    std::expected<void, FileError> from_file(const std::filesystem::path& file_path);

};
