#pragma once
#include <expected>
#include <filesystem>
#include <string>
#include <vector>

#include "config.hpp"
#include "utils.hpp"


class Tokenizer {
   private:
    struct M {
        std::vector<std::string> id_to_token{};
        //std::unordered_map<std::string, int16_t> token_to_id; TODO: Implement token_to_id in load function
        int vocab_size;
        int BOS = -1;
        int EOS = -1;
    } m;

    explicit Tokenizer(M m) : m(std::move(m)) {};

   public:
    static std::expected<Tokenizer, FileError> load(
        const ModelConfig& config, const std::filesystem::path& path);

    std::vector<int> encode(std::string text) const;
    std::string decode(const std::vector<int>& tokens) const;
    std::string decode_one(int token) const;


};
