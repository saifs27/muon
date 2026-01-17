#include "ops.hpp"
/*
void softmax(Tensor<std::bfloat16_t> x) {

    float max_value = x[0];
    auto size = x.size();
    for (int i = 0; i < size; i++) {
        if (x.raw_bytes[i] > max_value) max_value = i;
    }

    // sum all values
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += std::exp(max_value - x[i]);
    }

    for (int i = 0; i < size; i++) {
        x[i] = std::exp(max_value - x[i]) / sum;
    }
}
*/
