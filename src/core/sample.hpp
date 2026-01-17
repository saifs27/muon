#pragma once

struct Sampler {
    int vocab_size = 0;
    float sample(float* logits, float temperature = 1.0);
};