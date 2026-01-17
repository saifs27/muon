#include "ops_sycl.hpp"

void gelu(sycl::queue& q, const float* in, float* out, size_t N) {
    q.submit([&](sycl::handler& h) {
        h.parallel_for(sycl::range<1>(N), [=](sycl::id<1> idx) {
            size_t i = idx[0];
        });

    });
}
