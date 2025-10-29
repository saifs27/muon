#include "math_cpu.hpp"

void softmax(float* x, int size) {
    float max_value = x[0];
    for (int i = 0; i < size; i++) {
        if (x[i] > max_value) max_value = i;
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
