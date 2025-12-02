#include "math_cpu.hpp"

void softmax(Tensor<float>& x) {
    int size = x.size();
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

void rms_norm(
              const Tensor<float>& x,
              const Tensor<float>& w,
              Tensor<float>& y,
              float eps, int size) {
	float sum = 0;

	for (int i = 0; i < size; i++) {
		sum += x[i] * x[i];
	}

	float rms = sqrt(eps + sum / size);

	for (int i = 0; i < size; i++) {
		y[i] = (x[i] / rms) * w[i];
	}
}
