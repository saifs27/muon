#pragma once
#include <immintrin.h>

#include <string_view>

struct Config {};

struct Model {
    Config config;
    Model(Config config) : config(config) {};

    void forward();
    void generate(std::string_view prompt, int steps);
};
