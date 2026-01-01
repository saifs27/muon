#include <iostream>

#include "../core/model.hpp"
#include "../core/safetensors.hpp"
#include "../core/tokenizer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    const auto dir = argv[1];

    const auto sf = SafeTensors::load(dir);
    if (!sf.has_value()) {
        return 1;
    }

    const auto header_size = sf.value().header_size();

    std::cout << header_size << '\n';

    return 0;
}
