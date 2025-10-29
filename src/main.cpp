#include "math_cpu.hpp"
#include "tensor.hpp"
#include "tokenizer.hpp"
#include "model.hpp"

#include <print>


#include <iostream>

int main() {

    Tokenizer<4> tokenizer;

    tokenizer.vocab = {"Hello", "a", "b", "aa"};
    auto idx = {0, 2, 1, 3};
    auto n = tokenizer.decode(idx);

    std::println("hello");
    std::cout << n << '\n';

    return 0;

}
