#include "tokenizer.hpp"



std::string Tokenizer::decode(const std::vector<int>& tokens) const {
    std::string res;
    int string_size = std::accumulate(tokens.cbegin(), tokens.cend(), 0,
                                      [&](int sum, int idx) {
                                          sum += vocab[idx].size();
                                          return sum;
                                      });

    res.reserve(string_size);

    for (int i = 0, size = tokens.size(); i < size; i++) {
        res.append(vocab[tokens[i]]);
    }

    return res;
};

void Tokenizer::from_file(char * file) {
    using json = nlohmann::json;
    json data = json::parse(file);


}
