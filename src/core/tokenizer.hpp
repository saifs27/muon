#pragma once
#include <expected>
#include <filesystem>
#include <string>
#include <vector>

#include <json.hpp>

#include "config.hpp"
#include "utils.hpp"

struct TokenizerSAX : public nlohmann::json_sax<nlohmann::json> {
    std::vector<std::string>& vocab;
    const size_t vocab_size;

    bool in_model = false;
    bool in_vocab = false;
    bool in_added_tokens = false;
    bool in_added_token_object = false;

    int current_added_id = -1;
    std::string current_added_value;
    std::string current_key;

    int depth = 0;
    int model_depth = -1;
    int vocab_depth = -1;
    int added_tokens_depth = -1;

    TokenizerSAX(std::vector<std::string>& vocab, size_t vocab_size) :
        vocab(vocab), vocab_size(vocab_size) {}

    bool key(string_t& val) override {
        current_key = val;
        if (depth == 1 && val == "model") {
            in_model = true;
            model_depth = depth;
        }

        if (in_model && depth == model_depth + 1 && val == "vocab") {
            in_vocab = true;
            vocab_depth = depth + 1;
        }

        if (depth == 1 && val == "added_tokens") {
            in_added_tokens = true;
            added_tokens_depth = depth + 1;
        }
        return true;
    }
    bool number_unsigned(number_unsigned_t val) override { return handle_int(val);}
    bool number_integer(number_integer_t val) override {return handle_int(val);}

    bool handle_int(uint64_t val) {
        if (in_vocab && val < static_cast<uint64_t>(vocab_size)) {
            vocab[static_cast<size_t>(val)] = current_key;
        }

        if (in_added_token_object && current_key == "id") {
            current_added_id = static_cast<int>(val);
        }
        return true;
    }

    bool string(string_t& val) override {
        if (in_added_token_object) {
            if (current_key == "content") {
                current_added_value = val;
            }
        }
        return true;
    }

    bool start_object([[maybe_unused]] std::size_t elements) override {
        depth++;
        if (in_added_tokens && depth == added_tokens_depth + 1) {
            in_added_token_object = true;
            current_added_id = -1;
            current_added_value.clear();
        }
        return true;
    }

    bool end_object() override {
        if (in_added_token_object) {
            if (current_added_id != -1 && !current_added_value.empty() && current_added_id >= 0 && current_added_id < static_cast<int>(vocab_size)) {
                vocab[current_added_id] = current_added_value;
            }
            in_added_token_object = false;
        }

        if (in_vocab && depth == vocab_depth) {in_vocab = false;}
        if (in_model && depth == model_depth) {in_model = false;}
        
        depth--;
        return true;
    }

    bool start_array([[maybe_unused]] std::size_t elements) override {
        depth++;
        return true;
    }

    bool end_array() override {
        if (in_added_tokens && depth == added_tokens_depth) {
            in_added_tokens = false;
        }
        depth--;
        return true;
    }
    bool null() override {return true;}
    bool boolean([[maybe_unused]] bool val) override {return true;}
    bool number_float([[maybe_unused]] number_float_t val, [[maybe_unused]] const string_t& s) override {return true;}
    bool binary([[maybe_unused]] binary_t& val) override {return true;}
    bool parse_error([[maybe_unused]] std:: size_t position, [[maybe_unused]] const std::string& last_token, [[maybe_unused]] const nlohmann::detail::exception& ex) override {return false;}

};

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
