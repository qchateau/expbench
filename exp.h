#pragma once

#include <bit>
#include <cmath>

constexpr int K_TUNED_MINIMAX = 366393; // Max relative error: 2.98%
constexpr int K_TUNED_L1 = 545948; // Mean relative error: 1.48%
constexpr int K_TUNED_L2 = 486412; // Root mean squared relative error: 1.77%

// See https://specbranch.com/posts/fast-exp/
constexpr float very_fast_exp(float x, int K = K_TUNED_MINIMAX)
{
    float x_base = x * 12102203;
    int i_base = x_base;
    int i_result = i_base + (127 << 23) - K;
    return std::bit_cast<float>(i_result);
}

constexpr float P0 = 1.469318866729736328125;
constexpr float P1 = -0.671999752521514892578125;
constexpr float P2 = 0.22670517861843109130859375;

// See https://specbranch.com/posts/fast-exp/
constexpr float fast_exp(float x, int K = K_TUNED_MINIMAX)
{
    float x_base = x * 12102203;
    int i_base = x_base;
    int i_result = i_base + (127 << 23) - K;
    float first_order = std::bit_cast<float>(i_result);

    int correction_xi = (i_result & 0x7fffff) | (127 << 23);
    float correction_x = std::bit_cast<float>(correction_xi);

    float correction = std::fmaf(correction_x, P2, P1);
    correction = std::fmaf(correction, correction_x, P0);
    return first_order * correction;
}
