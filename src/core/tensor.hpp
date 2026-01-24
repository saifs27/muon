#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <span>
#include <flat_map>
#include <stdfloat>

enum class DType : uint8_t {
    fp32,
    fp16,
    bf16,
    i8,
    invalid
};

inline constexpr int dtype_to_n_bytes(DType dtype) {
    switch (dtype) {
        case DType::fp32 : return 4;
        case DType::fp16 : return 2;
        case DType::bf16 : return 2;
        case DType::i8 : return 1;
        case DType::invalid : return 0;
    }
};

inline DType str_to_tensor_dtype(const std::string_view dtype) {
    if (dtype == "FP32") {return DType::fp32;}
    if (dtype == "FP16") {return DType::fp16;}
    if (dtype == "BF16") {return DType::bf16;}
    if (dtype == "I8") {return DType::i8;}

    return DType::invalid;

}

enum class TensorError : uint8_t {
    DimensionError,
    IndexError,
};


// holds a view to tensor data to avoid repeated allocations.
template <typename T = std::bfloat16_t>
struct Tensor {
    std::span<T> data {};
    std::array<int, 4> shape {};

    Tensor() = default;

    // constructor for accepting raw bytes from safetensors file
    Tensor(const std::span<std::byte> raw_bytes, const std::array<int, 4> shape)
        : shape(shape) {
            auto count = raw_bytes.size_bytes() / sizeof(T);
            data = std::span<T>(std::bit_cast<T*>(raw_bytes.data()), count);
        }


    Tensor(const std::span<T> data, std::array<int, 4> shape) :
        data(data), shape(shape) {}



    std::span<T> view() const {
        return data;
    }

    int64_t size() const {
        return data.size();
    }

};

struct TensorArena {
    explicit TensorArena(size_t bytes) {
        memory.resize(bytes);
    }

    template <typename T>
    Tensor<T> allocate(std::array<int, 4> tensor_shape) {
        size_t n_elements = tensor_shape[0] * tensor_shape[1] * tensor_shape[2] * tensor_shape[3];
        size_t bytes = n_elements * sizeof(T);
        size_t alignment = 64;
        size_t padding = (alignment - (offset % alignment)) % alignment;
        
        if (offset + padding + bytes > memory.size()) {
            return Tensor<T>();
        }

        std::span<std::byte> bytes_span(memory.data() + offset, bytes);
        offset += padding;

        return Tensor<T>(bytes_span, tensor_shape);
    }

    void reset() { offset = 0; }

private:
    std::vector<std::byte> memory;
    size_t offset = 0;
};
