#pragma once
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>
#include <json.hpp>

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

    std::vector<int> encode(const std::string_view& text) const;
    std::string decode(const std::vector<int>& tokens) const;
    void from_file(char * file);

};
