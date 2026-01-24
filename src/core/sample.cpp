#include "sample.hpp"
#include <ranges>
#include <numeric>
#include <cassert>

float Sampler::sample(const Tensor<float> logits, float temperature) {
    float max_val = std::numeric_limits<float>::min();

    for (auto logit: logits.data) {
        if (logit > max_val) {
            max_val = logit;
        }
    }

    




    return 1.0;
}