#pragma once
#include <expected>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "config.hpp"
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

class Tokenizer {
   private:
    struct M {
        std::vector<std::string> vocab;
        int vocab_size;
        int BOS = -1;
        int EOS = -1;
    } m;

    explicit Tokenizer(M m) : m(std::move(m)) {};

   public:
    static std::expected<Tokenizer, FileError> load(
        const ModelConfig& config, const std::filesystem::path& path);

    std::vector<int> encode(const std::string text) const;
    std::string decode(const std::vector<int>& tokens) const;
    std::string decode_one(int token) const;


};
