#pragma once
#include <immintrin.h>
#include <vector>
#include <string_view>
#include "math_cpu.hpp"
#include "tokenizer.hpp"


struct Config {};

struct Block;

struct Model {
    Config config;
    std::vector<Block>& blocks;
    Model(Config config, std::vector<Block>& blocks) : config(config), blocks(blocks){};


    void forward(int token);
    void generate(std::string_view prompt, int steps);
};
