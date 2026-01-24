#pragma once
#include "tensor.hpp"

struct Sampler {
    int vocab_size = 0;
    float sample(Tensor<float> logits, float temperature = 1.0);
};
