#pragma once
#include <bit>


struct bfloat16{
    std::uint16_t bits;
};

inline float bf16_to_fp32(const bfloat16 bf16_bits) {
    auto fp32_bits = static_cast<uint32_t>(bf16_bits.bits) << 16;
    return std::bit_cast<float>(fp32_bits);
}

inline bfloat16 fp32_to_bf16(const float value) {
    auto fp32_bits = std::bit_cast<uint32_t>(value);
    auto bf16_bits = static_cast<uint16_t>(fp32_bits >> 16);

    return bfloat16(bf16_bits);
}