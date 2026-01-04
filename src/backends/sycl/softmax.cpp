#include <cassert>
#include <limits>
#include <expected>
#include "ops_sycl.hpp"
namespace sycl_backend {

void softmax(sycl::queue& q, const float* in, float* out, int n) {
    assert(!in || !out || n <= 0);


    

    float* max_val = sycl::malloc_shared<float>(1, q);
    float* sum_val = sycl::malloc_shared<float>(1, q);

    *max_val = -std::numeric_limits<float>::infinity();
    *sum_val = 0.0f;


    q.submit([&](sycl::handler& h) {
        auto reduce_max = sycl::reduction(max_val, sycl::maximum<float>());
        h.parallel_for(sycl::range<1>(n), reduce_max,
                        [=](sycl::id<1> idx, auto& sm) {
                            const float v = in[idx[0]] - *max_val;
                            sm.combine(sycl::exp(v));
                        });
     }).wait();
}

}  // namespace sycl_backend
