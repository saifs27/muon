#pragma once
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

struct Token {
    std::string_view str;
    int id;
};

struct TokenTrie {};

template <size_t vocab_size>
struct Tokenizer {
    std::array<std::string, vocab_size> vocab;
    int bos_token = -1;
    int eos_id = -1;
    int eot_token = -1;
    int byte_fallbacks = -1;
    // std::vector<int> encode(const std::string_view& text);

    std::string decode(const std::vector<int>& tokens) {
        std::string res;
        int string_size = std::accumulate(tokens.cbegin(), tokens.cend(), 0,
                                          [&](int sum, int idx) {
                                              sum += vocab[idx].size();
                                              return sum;
                                          });

        res.reserve(string_size);

        for (int idx : tokens) {
            res += vocab[idx];
        }

        return res;
    };
};
