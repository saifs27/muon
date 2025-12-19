#include "math_cpu.hpp"
#include "tensor.hpp"
#include "tokenizer.hpp"
#include "model.hpp"
#include "safetensors.hpp"

#include <print>

#include <iostream>

int main(int argc, char* argv[]) {
    
    if (argc != 2) return 1;
    auto dir = argv[1];

    Tokenizer tok(151936);

    auto result = tok.from_file(dir);

    if (!result.has_value()) {
        std::println("File error: {}", static_cast<int>(result.error()));
        return 1;
    }

    auto decoded = tok.decode({1, 2, 3, 100});

    std::println("{}", decoded);

    return 0;

}
